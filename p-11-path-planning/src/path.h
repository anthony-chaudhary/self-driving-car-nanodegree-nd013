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

		vector<double> jerk_minimal_trajectory(vector< double> start, vector <double> end, double T);



};

#endif // path_h