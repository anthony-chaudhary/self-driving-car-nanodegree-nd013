#ifndef path_h
#define path_h

#include <map>
#include <vector>
using namespace std;

class path {
public:

	path();
	virtual ~path();

	
	double timestep;
	double T;
	double trajectory_samples;
	vector<double> SIGMA_S, SIGMA_D;

	
	void init();

	vector<double> wiggle_goal(double t);

	double calculate_cost(vector<double> trajectory);
	void sensor_fusion_predict(vector< vector<double>> sensor_fusion);
	double nearest_approach_to_any_vehicle(vector<double> trajectory);
	
	double collision_cost(vector<double> trajectory);
	double coefficients_to_time_function(vector<double> coefficients, double t);

	void update_state();

	vector<double> trajectory_generation();

	vector<double> jerk_minimal_trajectory(vector< double> start, vector <double> end, double T);



};

class Vehicle : public path {

	public: 

		double nearest_approach(vector<double> trajectory, Vehicle vehicle);

		double radius = 1.5; // model vehicle as circle to simplify collision detection

		double s, s_dot, s_dot_dot, d, d_dot, d_dot_dot;
		double sf_x, sf_y, sf_vx, sf_vy, sf_s, sf_d;  // sensor fusion
		double sf_x_p, sf_y_p, sf_vx_p, sf_vy_p, sf_s_p, sf_d_p;  // previous sensor fusion readings
		string predicted_state;

		vector<double> S; // longitudinal 
		vector<double> D; // lateral

		double s_target, s_dot_target, d_target, d_dot_target;
		vector<double> S_TARGETS, D_TARGETS;

		void update_target_state(double t) {	
			this->s_target		= S[0] + (S[1] * t) + S[2] * pow(t, 2) / 2.0;
			this->s_dot_target	= S[1] + S[2] * t;
			this->d_target		= D[0] + (D[1] * t) + D[2] * pow(t, 2) / 2.0;
			this->d_dot_target	= D[1] + D[2] * t;
			S_TARGETS = { s_target, s_dot_target, S[2] };
			D_TARGETS = { d_target, d_dot_target, D[2] };
		}

};

#endif // path_h