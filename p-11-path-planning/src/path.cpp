#include "path.h"
#include "classifier.h"
#include "Eigen-3.3/Eigen/Dense"
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <random>
#define _USE_MATH_DEFINES
#include <math.h>
#include "spline.h"
constexpr double pi() { return M_PI; }

path::path() {}
path::~path() {}

Vehicle *r_daneel_olivaw		= new Vehicle;  // our self driving car
GNB		*classifier			= new GNB;
Vehicle *target				= new Vehicle;
path		*our_path			= new path;

map<int, Vehicle>				other_vehicles;
vector < path::Weighted_costs > weighted_costs;
default_random_engine			generator;

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;

double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

void path::init() {
	
	vector< vector<double> > X_train = classifier->load_state("./train_states.txt");
	vector< string > Y_train = classifier->load_label("./train_labels.txt");
	classifier->train(X_train, Y_train);

	weighted_costs.resize(3);
	weighted_costs[0].weight = 1.0;
	weighted_costs[1].weight = 1.0;
	weighted_costs[2].weight = 1.0;
	
	our_path->timestep = .02;
	our_path->T = 1.5;
	our_path->trajectory_samples = 10;
	our_path->SIGMA_S = { 10.0, 2.0, .50 };
	our_path->SIGMA_D = { 0.01, .001, .0001 };

}

void path::sensor_fusion_predict(vector< vector<double>> sensor_fusion) {
	// Store raw sensor_fusion observations and make a prediction 

	// 1. Update vehicles list
	for (size_t i = 0; i < sensor_fusion.size(); ++i) {
		
		int id = sensor_fusion[i][0];
		if (other_vehicles.find(id) == other_vehicles.end()) {
			// if vehicle doesn't exist create a new one & init
			Vehicle *vehicle = new Vehicle;
			vehicle->update_sensor_fusion(sensor_fusion, i);
			other_vehicles.insert(make_pair(id, *vehicle));
		}

		// 2. Update previous sensor readings
		Vehicle *vehicle = &other_vehicles.find(id)->second;
		vehicle->update_sensor_fusion_previous();

		// 3. Update new sensor readings
		vehicle->update_sensor_fusion(sensor_fusion, i);
		
		// 4. Run classifier for prediction
		vehicle->predicted_state = classifier->predict(vehicle->D[1]);
	}
	
}


void path::update_our_car_state(double car_x, double car_y, double car_s, double car_d, 
	double car_yaw, double car_speed) {
/****************************************
* Behavior planning usine finite state machine
in progress
****************************************/

	// previous
	r_daneel_olivaw->S_p = r_daneel_olivaw->S;
	r_daneel_olivaw->D_p = r_daneel_olivaw->D;

	// new readings
	r_daneel_olivaw->S[0] = car_s;
	r_daneel_olivaw->D[0] = car_d;

	if (car_speed != 0) {
		r_daneel_olivaw->S[1] = r_daneel_olivaw->S[0] - r_daneel_olivaw->S_p[0];
		r_daneel_olivaw->D[1] = r_daneel_olivaw->D[0] - r_daneel_olivaw->D_p[0];

		r_daneel_olivaw->S[2] = r_daneel_olivaw->S[1] - r_daneel_olivaw->S_p[1];  // ie 100 - 90 = change of 10
		r_daneel_olivaw->D[2] = r_daneel_olivaw->D[1] - r_daneel_olivaw->D_p[1];
	}

	r_daneel_olivaw->x = car_x;
	r_daneel_olivaw->y = car_y;
	r_daneel_olivaw->yaw = car_yaw;
	r_daneel_olivaw->speed = car_speed;	 // storing twice?

	
	// &other_vehicles[4];   // hard coded, target could also be x,y / d,s ?
	
	target->S[0] = car_s + 10;
	target->S[1] = .02;  /// this would have relationship to S....

	target->D[0] = car_d;
	

	target->D[1] = 0;
	target->D[2] = 0;
	target->update_target_state(our_path->timestep);

}


vector<double> path::trajectory_generation() {
/****************************************
* find best trajectory according to weighted cost function
****************************************/

	// 1. Generate random nearby goals
	vector< vector<double>> goals;

	// first goal
	goals.resize(1);
	goals[0].insert(end(goals[0]), begin(target->S_TARGETS), end(target->S_TARGETS));
	goals[0].insert(end(goals[0]), begin(target->D_TARGETS), end(target->D_TARGETS));

	double t = our_path->T - 2 * our_path->timestep;
	double b = our_path->T + 2 * our_path->timestep;;
	goals[0].push_back(t);

	// other goals
	while (t <= b) {

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
	start_d = { r_daneel_olivaw->D[0], r_daneel_olivaw->D[1], r_daneel_olivaw->D[2] };

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
	
		normal_distribution<double> distribution(target->S_TARGETS[i], our_path->SIGMA_S[i]);
		new_goal[i] = distribution(generator);
	}
	for (size_t i = 0; i < 3; ++i) {

		normal_distribution<double> distribution(target->D_TARGETS[i], our_path->SIGMA_D[i]);
		new_goal[i+3] = distribution(generator);
	}
	new_goal[6] = t;
	return new_goal;

}

double path::calculate_cost(vector<double> trajectory) {
	double cost = 0;
	for (size_t i = 0; i < 1; ++i) {
		cost += weighted_costs[0].weight * collision_cost(trajectory);
		//cost += weighted_costs[1].weight * s_diff_cost(trajectory);
		//cost += weighted_costs[2].weight * d_diff_cost(trajectory);
		//cost += new_cost;
	}
	return cost;
}


path::Previous_path path::merge_previous_path(path::MAP *MAP, vector< double> previous_path_x,
	vector< double> previous_path_y, double car_yaw, double car_s, double car_d, double end_path_s, double end_path_d) {
	
	path::Previous_path Previous_path;
	int p_x_size, i_p_x;
	p_x_size		= previous_path_x.size();
	//i_p_x		= p_x_size - 40;
	//i_p_x		= min(i_p_x, 100);

	if (p_x_size != 0) {

		cout << "i_p_x " << i_p_x << endl;
		cout << previous_path_x[i_p_x] << "\t" << previous_path_y[i_p_x] << endl;
		
		vector<double> new_s_d = getFrenet(previous_path_x[i_p_x -1], previous_path_y[i_p_x -1], car_yaw, MAP->waypoints_x_upsampled, MAP->waypoints_y_upsampled);
		
		Previous_path.s = new_s_d[0];
		Previous_path.d = new_s_d[1];

		//Previous_path.s = end_path_s;
		//Previous_path.d = end_path_d;

		cout << "calucated car_s" << car_s << " car_d " << car_d << endl;

		for (size_t i = 0; i < i_p_x; i++) {
			Previous_path.X.push_back(previous_path_x[i]);
			Previous_path.Y.push_back(previous_path_y[i]);
		}
	}
	else {  // Init case
		Previous_path.s = car_s;
		Previous_path.d = car_d;
	}
	return Previous_path;
	
}

path::X_Y path::convert_new_path_to_X_Y_and_merge(path::MAP* MAP, path::S_D S_D_, path::Previous_path Previous_path) {

	path::X_Y X_Y, X_Y_spline, X_Y_generated;
	X_Y.X = Previous_path.X;
	X_Y.Y = Previous_path.Y;
	int x_size = Previous_path.X.size();

	tk::spline spline_x, spline_y;

	for (size_t i = 0; i < S_D_.D.size(); ++i) {

		vector<double> a = getXY(S_D_.S[i], S_D_.D[i], MAP->waypoints_s_upsampled,
			MAP->waypoints_x_upsampled, MAP->waypoints_y_upsampled);
		/*
		if (x_size != 0) {
			if (i == 0) {

				vector<double> x, y;
				x.push_back(a[0]);
				y.push_back(a[1]);
				for (size_t i = 0; i < 3; ++i) {
					x.push_back(X_Y.X[x_size - 1] + (.001 * i));
					y.push_back(X_Y.Y[x_size - 1] + (.001 * i));
				}

				sort(x.begin(), x.end());
				sort(y.begin(), y.end());

				spline_x.set_points(x, y);
				spline_y.set_points(y, x);

				for (size_t i = 2; i < x.size() - 1; ++i) {
					X_Y.X.push_back(spline_y(y[i]));
					X_Y.Y.push_back(spline_x(x[i]));
				}
			}
		}
		*/

		X_Y.X.push_back(a[0]);
		X_Y.Y.push_back(a[1]);

	}

	return X_Y;
}
	/*
	if (x_size != 0) {

	
		for (size_t i = 0; i < x_size; ++i) {

			normal_distribution<double> distribution_x(X_Y.X[i], .000000001);
			X_Y.X[i] = distribution_x(generator);
			
			normal_distribution<double> distribution_y(X_Y.Y[i], .000000001);
			X_Y.Y[i] = distribution_y(generator);

		}

		sort(X_Y.X.begin(), X_Y.X.end());
		sort(X_Y.Y.begin(), X_Y.Y.end());

		spline_x.set_points(X_Y.X, X_Y.Y);
		spline_y.set_points(X_Y.Y, X_Y.X);
		
		double x_p, y_p;
		double merge_points;

		int merged_x_size = X_Y.X.size();

		for (size_t i = 0; i < merged_x_size; ++i) {

			X_Y_spline.X.push_back(spline_y(X_Y.Y[i]));
			X_Y_spline.Y.push_back(spline_x(X_Y.X[i]));

		}
		return X_Y_spline;
	}
	else {
		return X_Y;
	}
	*/



	// merge

	/*// Method one
	double x_mean, y_mean;
	x_mean = 0;
	y_mean = 0;
	int x_size = Previous_path.X.size();
	int merge_size = 3;

	for (size_t i = x_size - merge_size;  i < x_size + merge_size; ++i){
		x_mean += X_Y.X[i];
		y_mean += X_Y.Y[i];
	}

	x_mean = x_mean / (merge_size * 2);
	y_mean = y_mean / (merge_size * 2);

	for (size_t i = x_size; i < x_size + merge_size; ++i) {
		X_Y.X[i] = X_Y.X[i] * .70 + x_mean * .30 ;
		X_Y.Y[i] = X_Y.Y[i] * .70 + y_mean * .30 ;
	}
	*/



/****************************************
* Cost functions
****************************************/

double path::s_diff_cost(vector<double> trajectory) {
	vector<double> S; // TODO better way to do this
	S = { trajectory[0], trajectory[1], trajectory[2], trajectory[3], trajectory[4], trajectory[5] };
	double T = trajectory[12];


	return 0;
	// WIP
}

double path::d_diff_cost(vector<double> trajectory) {

	// WIP
	return 0;
}


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

	vector<double> S, D; // TODO better way to do this
	S = { trajectory[0], trajectory[1], trajectory[2], trajectory[3], trajectory[4], trajectory[5] };
	D = { trajectory[6], trajectory[7], trajectory[8], trajectory[9], trajectory[10], trajectory[11] };

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

path::S_D  path::build_trajectory(vector<double> trajectory) {
	
	vector<double> S, D, X, Y; // TODO better way to do this
	// refactor using S_D struct
	S = { trajectory[0], trajectory[1], trajectory[2], trajectory[3], trajectory[4], trajectory[5] };
	D = { trajectory[6], trajectory[7], trajectory[8], trajectory[9], trajectory[10], trajectory[11] };

	S_D S_D_;
	double t = 0;
	while (t <= our_path->T ){
		S_D_.S.push_back(coefficients_to_time_function(S, t));
		S_D_.D.push_back(coefficients_to_time_function(D, t));
		t += our_path->timestep;
	}
	return S_D_;

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


// Transform from Cartesian x,y coordinates to Frenet s,d coordinates
vector<double> path::getFrenet(double x, double y, double theta, vector<double> maps_x, vector<double> maps_y)
{
	int next_wp = NextWaypoint(x, y, theta, maps_x, maps_y);

	int prev_wp;
	prev_wp = next_wp - 1;
	if (next_wp == 0)
	{
		prev_wp = maps_x.size() - 1;
	}

	double n_x = maps_x[next_wp] - maps_x[prev_wp];
	double n_y = maps_y[next_wp] - maps_y[prev_wp];
	double x_x = x - maps_x[prev_wp];
	double x_y = y - maps_y[prev_wp];

	// find the projection of x onto n
	double proj_norm = (x_x*n_x + x_y*n_y) / (n_x*n_x + n_y*n_y);
	double proj_x = proj_norm*n_x;
	double proj_y = proj_norm*n_y;

	double frenet_d = distance(x_x, x_y, proj_x, proj_y);

	//see if d value is positive or negative by comparing it to a center point

	double center_x = 1000 - maps_x[prev_wp];
	double center_y = 2000 - maps_y[prev_wp];
	double centerToPos = distance(center_x, center_y, x_x, x_y);
	double centerToRef = distance(center_x, center_y, proj_x, proj_y);

	if (centerToPos <= centerToRef)
	{
		frenet_d *= -1;
	}

	// calculate s value
	double frenet_s = 0;
	for (int i = 0; i < prev_wp; i++)
	{
		frenet_s += distance(maps_x[i], maps_y[i], maps_x[i + 1], maps_y[i + 1]);
	}

	frenet_s += distance(0, 0, proj_x, proj_y);

	return { frenet_s,frenet_d };

}

// Transform from Frenet s,d coordinates to Cartesian x,y
vector<double> path::getXY(double s, double d, vector<double> maps_s, vector<double> maps_x, vector<double> maps_y)
{
	int prev_wp = -1;

	while (s > maps_s[prev_wp + 1] && (prev_wp < (int)(maps_s.size() - 1)))
	{
		prev_wp++;
	}

	int wp2 = (prev_wp + 1) % maps_x.size();

	double heading = atan2((maps_y[wp2] - maps_y[prev_wp]), (maps_x[wp2] - maps_x[prev_wp]));
	// the x,y,s along the segment
	double seg_s = (s - maps_s[prev_wp]);

	double seg_x = maps_x[prev_wp] + seg_s*cos(heading);
	double seg_y = maps_y[prev_wp] + seg_s*sin(heading);

	double perp_heading = heading - pi() / 2;

	double x = seg_x + d*cos(perp_heading);
	double y = seg_y + d*sin(perp_heading);

	return { x,y };

}

double path::distance(double x1, double y1, double x2, double y2)
{
	return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}
int path::ClosestWaypoint(double x, double y, vector<double> maps_x, vector<double> maps_y)
{

	double closestLen = 100000; //large number
	int closestWaypoint = 0;

	for (int i = 0; i < maps_x.size(); i++)
	{
		double map_x = maps_x[i];
		double map_y = maps_y[i];
		double dist = distance(x, y, map_x, map_y);
		if (dist < closestLen)
		{
			closestLen = dist;
			closestWaypoint = i;
		}
	}

	return closestWaypoint;
}
int path::NextWaypoint(double x, double y, double theta, vector<double> maps_x, vector<double> maps_y)
{

	int closestWaypoint = ClosestWaypoint(x, y, maps_x, maps_y);

	double map_x = maps_x[closestWaypoint];
	double map_y = maps_y[closestWaypoint];

	double heading = atan2((map_y - y), (map_x - x));

	double angle = abs(theta - heading);

	if (angle > pi() / 4)
	{
		closestWaypoint++;
	}

	return closestWaypoint;

}