#ifndef path_h
#define path_h

#include <vector>

class path {
	public:

		path();
		virtual ~path();


		void init();

		//classifier

		void sensor_fusion_predict();

		void update_state();

		void trajectory_generation();

		vector<double> jerk_minimal_trajectory(vector< double> start, vector <double> end, double T);



};

class Vehicle : public path {

	public: 

		double radius = 1.5; // model vehicle as circle to simplify collision detection

};

#endif // path_h