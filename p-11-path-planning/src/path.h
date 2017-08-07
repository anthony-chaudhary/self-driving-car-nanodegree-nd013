#ifndef path_h
#define path_h

#include <chrono>
#include <map>
#include <vector>
#include <ctime>

using namespace std;

class path {
public:

	path();
	virtual ~path();


	
	// Data structures
	struct Weighted_costs {
		double weight;
		// function
	};
	struct X_Y {
		vector<double> X;
		vector<double> Y;
	};
	struct S_D {
		vector<double> S;
		vector<double> D;
	};
	struct Previous_path {
		vector<double> X;
		vector<double> Y;
		double s;
		double d;
	};
	struct MAP {
		vector<double> waypoints_s_upsampled = {};
		vector<double> waypoints_x_upsampled = {};
		vector<double> waypoints_y_upsampled = {};
	};


	// Shared variables
	chrono::system_clock::time_point start_time, current_time, mpc_clock;
	int previous_path_keeps;
	vector<double> last_trajectory;
	double timestep;
	double T;
	double distance_goal;
	int trajectory_samples;
	vector<double> SIGMA_S, SIGMA_D;

	// Cost functions
	double calculate_cost(vector<double> trajectory);
	double efficiency_cost(vector<double> trajectory);
	double collision_cost(vector<double> trajectory);
	double d_diff_cost(vector<double> trajectory);
	double max_acceleration_cost(vector<double> trajectory);
	double total_acceleration_cost(vector<double> trajectory);
	double total_jerk_cost(vector<double> trajectory);
	double buffer_cost(vector<double> trajectory);
	double s_diff_cost(vector<double> trajectory);
	double speed_limit_cost(vector<double> trajectory);
	double max_jerk_cost(vector<double> trajectory);
	double stay_in_lane(vector<double> trajectory);


	// Helper functions
	void init();
	double coefficients_to_time_function(vector<double> coefficients, double t);
	double logistic(double x);
	vector<double> wiggle_goal(double t);
	vector<double> differentiate_polynomial(vector<double> coefficients);
	vector<double> getFrenet(double x, double y, double theta, vector<double> maps_x, vector<double> maps_y);
	vector<double> getXY(double s, double d, vector<double> maps_s, vector<double> maps_x, vector<double> maps_y);
	double distance(double x1, double y1, double x2, double y2);
	int ClosestWaypoint(double x, double y, vector<double> maps_x, vector<double> maps_y);
	int NextWaypoint(double x, double y, double theta, vector<double> maps_x, vector<double> maps_y);
	vector<double> get_ceoef_and_rates_of_change(vector<double> coefficients);
	double nearest_approach_to_any_vehicle(vector<double> trajectory);

	// Path functions
	void update_our_car_state(MAP *MAP, double car_x, double car_y, double car_s, double car_d,
		double car_yaw, double car_speed);
	void sensor_fusion_predict(vector< vector<double>> sensor_fusion);
	vector<double> trajectory_generation();
	vector<double> jerk_minimal_trajectory(vector< double> start, vector <double> end, double T);
	Previous_path merge_previous_path(MAP *MAP, vector< double> previous_path_x,
		vector< double> previous_path_y, double car_yaw, double car_s, double car_d, double end_path_s, double end_path_d);
	X_Y convert_new_path_to_X_Y_and_merge(MAP *MAP, S_D S_D_, Previous_path Previous_path);
	S_D build_trajectory(vector<double> trajectory);

};

class Vehicle : public path {

public:

	double nearest_approach(vector<double> trajectory, Vehicle vehicle);

	double radius = 1.5; // model vehicle as circle to simplify collision detection

	double x, y, yaw, speed;
	double sf_x, sf_y, sf_vx, sf_vy, sf_s, sf_d;  // sensor fusion
	double sf_x_p, sf_y_p, sf_vx_p, sf_vy_p, sf_s_p, sf_d_p;  // previous sensor fusion readings
	string predicted_state;

	vector<double> S = { 0, 0, 0 }; // longitudinal    S, S_dot, S_dot_dot  s_position, velocity, acceleration
	vector<double> D = { 0, 0, 0 }; // lateral		D, D_dot, D_dot_dot
	vector<double> S_p = { 0, 0, 0 }; // previous longitudinal 
	vector<double> D_p = { 0, 0, 0 }; // previous lateral

	
	double s_target, s_dot_target, d_target, d_dot_target;
	vector<double> S_TARGETS, D_TARGETS;

	void update_sensor_fusion(vector< vector<double>> sensor_fusion, int index) {
		this->sf_x = sensor_fusion[index][1];
		this->sf_y = sensor_fusion[index][2];
		this->sf_vx = sensor_fusion[index][3];
		this->sf_vy = sensor_fusion[index][4];
		this->sf_s = sensor_fusion[index][5];
		this->sf_d = sensor_fusion[index][6];

		this->S[0] = this->sf_s;
		this->D[0] = this->sf_d;

		// TODO Convert x,y velocity to s,d frame??
		this->S[1] = this->S[0] - this->S_p[0];
		this->D[1] = this->D[0] - this->D_p[0];
		this->S[2] = this->S[1] - this->S_p[1];  // ie 100 - 90 = change of 10
		this->D[2] = this->D[1] - this->D_p[1];

	}

	void update_sensor_fusion_previous() {
		this->sf_x_p = this->sf_x;
		this->sf_y_p = this->sf_y;
		this->sf_vx_p = this->sf_vx;
		this->sf_vy_p = this->sf_vy;
		this->sf_s_p = this->sf_s;
		this->sf_d_p = this->sf_d;

		this->S_p = this->S;
		this->D_p = this->D;

	}

	void update_target_state(double t) {
		this->s_target = S[0] + (S[1] * t) + S[2] * (t * t) / 2.0;
		this->s_dot_target = S[1] + S[2] * t;
		this->d_target = D[0] + (D[1] * t) + D[2] * (t * t) / 2.0;
		this->d_dot_target = D[1] + D[2] * t;
		S_TARGETS = { s_target, s_dot_target, S[2] };
		D_TARGETS = { d_target, d_dot_target, D[2] };
	}

};

#endif // path_h
