#include "path.h"
#include "classifier.h"
#include "Eigen-3.3/Eigen/Dense"
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <random>

path::path() {}
path::~path() {}

Vehicle *r_daneel_olivaw = new Vehicle;  // our self driving car
default_random_engine generator;
GNB *classifier = new GNB;

map<int, Vehicle> other_vehicles;
Vehicle *target;  // vehicle target
path *our_path = new path;
vector < path::Weighted_costs > weighted_costs;

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;

void path::init() {
	
	/****************************************
	* 1. Prediction - classifier
	****************************************/
	vector< vector<double> > X_train = classifier->load_state("./train_states.txt");
	vector< string > Y_train = classifier->load_label("./train_labels.txt");
	classifier->train(X_train, Y_train);

	weighted_costs.resize(1);
	weighted_costs[0].weight = 1.0;

	r_daneel_olivaw->S_p = { 0, 0, 0 }; 
	r_daneel_olivaw->D_p = { 0, 0, 0 };
	r_daneel_olivaw->S = { 0, 0, 0 };
	r_daneel_olivaw->D = { 0, 0, 0 };
	
	our_path->timestep = .5;
	our_path->T = 5.0;
	our_path->trajectory_samples = 10;
	our_path->SIGMA_S = { 10.0, 4.0, 2.0 };
	our_path->SIGMA_D = { 1.0, 1.0, 1.0 };

}

void path::sensor_fusion_predict(vector< vector<double>> sensor_fusion) {
/****************************************
*  Turn raw sensor_fusion observations
 into predictions for use by update_state() and trajectory_generation() 
****************************************/

	// 1. Update vehicles list  (create new objects?)
	for (size_t i = 0; i < sensor_fusion.size(); ++i) {
		
		int id = sensor_fusion[i][0];
		if (other_vehicles.find(id) == other_vehicles.end()) {
			
			// if vehicle doesn't exist create a new one
			Vehicle *vehicle = new Vehicle;
			vehicle->sf_x, vehicle->sf_y, vehicle->sf_vx, 
				vehicle->sf_vy, vehicle->sf_s, vehicle->sf_d = 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;
			vehicle->S_p = { 0, 0, 0 };
			vehicle->D_p = { 0, 0, 0 };
			vehicle->S = { 0, 0, 0 };
			vehicle->D = { 0, 0, 0 };
			other_vehicles.insert(make_pair(id, *vehicle));
		}

		// get vehicle pointer
		Vehicle *vehicle = &other_vehicles.find(id)->second;

		// 2. Update sensor readings
		// previous
		vehicle->sf_x_p = vehicle->sf_x;
		vehicle->sf_y_p = vehicle->sf_y;
		vehicle->sf_vx_p = vehicle->sf_vx;
		vehicle->sf_vy_p = vehicle->sf_vy;
		vehicle->sf_s_p = vehicle->sf_s;
		vehicle->sf_d_p = vehicle->sf_d;

		// new
		vehicle->sf_x	= sensor_fusion[i][1];
		vehicle->sf_y	= sensor_fusion[i][2];
		vehicle->sf_vx	= sensor_fusion[i][3];
		vehicle->sf_vy	= sensor_fusion[i][4];
		vehicle->sf_s	= sensor_fusion[i][5];
		vehicle->sf_d	= sensor_fusion[i][6];

		vehicle->S[0] = vehicle->sf_s;
		vehicle->D[0] = vehicle->sf_d;

		// TODO Convert x,y velocity to s,d frame??

		vehicle->S[1] = vehicle->S_p[0] - vehicle->S[0];
		vehicle->D[1] = vehicle->D_p[0] - vehicle->D[0];

		vehicle->S[2] = vehicle->S_p[1] - vehicle->S[1];
		vehicle->D[2] = vehicle->D_p[1] - vehicle->D[1];

		double d_dot = vehicle->D[1];   // is this right?
		
		// run  classifier->predict() to see if changing lanes?
		string prediction = classifier->predict(d_dot);
		vehicle->predicted_state = prediction;
	
	}
}


void path::update_our_car_state(double car_x, double car_y, double car_s, double car_d, 
	double car_yaw, double car_speed) {
/****************************************
* Behavior planning usine finite state machine
****************************************/

	r_daneel_olivaw->S[0] = car_s;
	r_daneel_olivaw->D[0] = car_d;

	r_daneel_olivaw->S[1] = car_speed;
	r_daneel_olivaw->D[1] = r_daneel_olivaw->D_p[0] - r_daneel_olivaw->D[0];

	r_daneel_olivaw->x = car_x;
	r_daneel_olivaw->y = car_y;
	r_daneel_olivaw->yaw = car_yaw;
	r_daneel_olivaw->speed = car_speed;	

	target = &other_vehicles[0];
	target->update_target_state(100);

}


vector<double> path::trajectory_generation() {
/****************************************
* find best trajectory according to weighted cost function
****************************************/


	// 1. Generate random nearby goals

	vector< vector<double>> goals;
	// First goal
	goals.resize(1);
	goals[0].insert(end(goals[0]), begin(target->S_TARGETS), end(target->S_TARGETS));
	goals[0].insert(end(goals[0]), begin(target->D_TARGETS), end(target->D_TARGETS));

	double t = our_path->T - 4 * our_path->timestep;
	double b = t;
	goals[0].push_back(t);

	while (t <= b) {

		// other goals
		target->update_target_state(t);
		vector< vector<double>> target_goals;
		vector<double> new_goal;
		target_goals.resize(our_path->trajectory_samples);
		
		for (size_t i = 0; i < target_goals.size(); ++i) {
			target_goals[i] = wiggle_goal(t);
		}
		
		goals.insert(end(goals), begin(target_goals), end(target_goals));
		t += our_path->timestep;
	}

	// 2. Store jerk minimal trajectories for all goals
	vector< vector<double>> trajectories;
	trajectories.resize(goals.size());

	vector<double> s_goal, d_goal, s_coeffecients, d_coeffecients,
		start_s, start_d;
	double t_2;
	start_s = { r_daneel_olivaw->S[0], r_daneel_olivaw->S[1], r_daneel_olivaw->S[2] };
	start_d = { r_daneel_olivaw->D[0], r_daneel_olivaw->D[0], r_daneel_olivaw->D[2] };

	for (size_t i = 0; i < goals.size(); ++i) {
		
		s_goal = { goals[i][0], goals[i][1], goals[i][2] };
		d_goal = { goals[i][3], goals[i][4], goals[i][5] };
		t_2 = goals[i][6];

		s_coeffecients = jerk_minimal_trajectory(start_s, s_goal, t_2);
		d_coeffecients = jerk_minimal_trajectory(start_d, d_goal, t_2);
		
		trajectories[i].insert(end(trajectories[i]), begin(s_coeffecients), end(s_coeffecients) );
		trajectories[i].insert(end(trajectories[i]), begin(d_coeffecients), end(d_coeffecients) );
		trajectories[i].push_back(t);
	}

	// 3. Find best using weighted cost function
	double min_cost = 1e10;
	double cost;
	vector<double> best_trajectory;
	for (size_t i = 0; i < trajectories.size(); ++i) {

		cost = calculate_cost(trajectories[i]);
		if (cost < min_cost) {
			min_cost = cost;
			best_trajectory = trajectories[i];
		}
	}

	return best_trajectory;

}

vector<double> path::wiggle_goal(double t) {
	
	vector<double> new_goal(7);
	for (size_t i = 0; i < 3; ++i) {
	
		normal_distribution<double> distribution(target->D_TARGETS[i], our_path->SIGMA_D[i]);
		new_goal[i] = distribution(generator);
	}
	for (size_t i = 0; i < 3; ++i) {

		normal_distribution<double> distribution(target->S_TARGETS[i], our_path->SIGMA_S[i]);
		new_goal[i+3] = distribution(generator);
	}
	new_goal[6] = t;
	return new_goal;

}

double path::calculate_cost(vector<double> trajectory) {
	double cost = 0;
	for (size_t i = 0; i < 1; ++i) {
		double new_cost = weighted_costs[0].weight * collision_cost(trajectory);
		cost += new_cost;
	}
	return cost;
}

/****************************************
* Cost functions
****************************************/

double path::collision_cost(vector<double> trajectory) {

	double a = nearest_approach_to_any_vehicle(trajectory);
	double b = 2 * r_daneel_olivaw->radius;
	if (a < b) { return 1.0;	 }
	else { return 0.0; }
}

double path::nearest_approach_to_any_vehicle(vector<double> trajectory) {
// returns closest distance to any vehicle

	double a = 1e9;
	for (size_t i = 0; i < other_vehicles.size(); ++i) {
		double b = r_daneel_olivaw->nearest_approach(trajectory, other_vehicles[i]);
		if (b < a) { a = b; }
	}
	return a;

}

double Vehicle::nearest_approach(vector<double> trajectory, Vehicle vehicle) {
	
	double T, s_time, d_time, a, b, c, e, t;
	a = 1e9;
	T = trajectory[12];

	vector<double> S, D;
	S = { trajectory[0], trajectory[1], trajectory[2] };
	D = { trajectory[6], trajectory[7], trajectory[8] };

	for (size_t i = 0; i < 100; ++i) {
		t = double(i) / 100 * T;
		s_time = coefficients_to_time_function(D, t);
		d_time = coefficients_to_time_function(S, t);

		vehicle.update_target_state(t);

		b = pow((s_time - vehicle.s_target), 2);
		c = pow((d_time - vehicle.d_target), 2);
		e = sqrt(b + c);

		if (e < a) { a = e; }
	}
	return a;
}


double path::coefficients_to_time_function(vector<double> coefficients, double t) {
	// Returns a function of time given coefficients
	double total = 0.0;
	for (size_t i = 0; i < coefficients.size(); ++i) {
		total += coefficients[i] * pow(t, i);
	}
	return total;
}


vector<double> path::jerk_minimal_trajectory(vector< double> start, vector <double> end, double T)
{
	/*
	Calculate the Jerk Minimizing Trajectory that connects the initial state
	to the final state in time T.

	INPUTS

	start - the vehicles start location given as a length three array
	corresponding to initial values of [s, s_dot, s_double_dot]

	end   - the desired end state for vehicle. Like "start" this is a
	length three array.

	T     - The duration, in seconds, over which this maneuver should occur.

	OUTPUT
	an array of length 6, each value corresponding to a coefficent in the polynomial
	s(t) = a_0 + a_1 * t + a_2 * t**2 + a_3 * t**3 + a_4 * t**4 + a_5 * t**5

	EXAMPLE

	> JMT( [0, 10, 0], [10, 10, 0], 1)
	[0.0, 10.0, 0.0, 0.0, 0.0, 0.0]
	*/

	const double s_i = start[0];
	const double s_i_dot = start[1];
	const double s_i_dot_dot = start[2] / 2;

	const double s_f = end[0];
	const double s_f_dot = end[1];
	const double s_f_dot_dot = end[2];

	const double s_r_1 = s_f - (s_i + s_i_dot * T + (s_i_dot_dot * T * T) / 2);
	const double s_r_2 = s_f_dot - (s_i_dot + s_i_dot_dot * T);
	const double s_r_3 = s_f_dot_dot - s_i_dot_dot;

	VectorXd t_1(3), t_2(3), t_3(3);
	t_1 << pow(T, 3), pow(T, 4), pow(T, 5);
	t_2 << 3 * pow(T, 2), 4 * pow(T, 3), 5 * pow(T, 4);
	t_3 << 6 * pow(T, 1), 12 * pow(T, 2), 20 * pow(T, 3);

	MatrixXd s_matrix(3, 1), T_matrix(3, 3);
	s_matrix << s_r_1, s_r_2, s_r_3;
	T_matrix.row(0) = t_1;
	T_matrix.row(1) = t_2;
	T_matrix.row(2) = t_3;

	MatrixXd T_inverse = T_matrix.inverse();

	MatrixXd output(3, 1);
	output = T_inverse * s_matrix;

	double a_3 = double(output.data()[0]);
	double a_4 = double(output.data()[1]);
	double a_5 = double(output.data()[2]);

	vector<double> results;
	results = { s_i, s_i_dot, s_i_dot_dot, a_3, a_4, a_5 };

	// cout << output << endl;

	return results;

}
