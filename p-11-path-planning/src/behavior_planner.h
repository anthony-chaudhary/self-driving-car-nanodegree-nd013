#ifndef BEHAVIOR_PLANNER_H
#define BEHAVIOR_PLANNER_H
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
using namespace std;
#include <chrono>
#include "path.h"

class Behavior {

public:
	Behavior();
	virtual ~Behavior();

	struct lane {
		int id;
		float cost = 0;  
		int d;  // d centre
		int d_upper;  // d upper
		int d_lower;  // d lower
	};

	struct lanes {
		vector<lane> L;
	};

	struct state {

		lane L;  // current
		lane L_target;  // target
		chrono::steady_clock::time_point lane_change_end_time;
	};

	void init();
	lane update_behavior_state(vector<double> trajectory, path *our_path);
	void find_best_lane();
	void update_lane_costs(vector<double> trajectory, path *our_path);

};

#endif