#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include "behavior_planner.h"
#include <algorithm>
#include "path.h"

using namespace std;

Behavior::Behavior() {};
Behavior::~Behavior() {};

Behavior::lanes *road = new Behavior::lanes;
Behavior::lanes *cars = new Behavior::lanes;

Behavior::state *State = new Behavior::state;


void Behavior::init() {

	for (size_t i = 0; i < 3; ++i) {
		lane l_;
		l_.id = i;
		l_.d_lower = i * 4;
		l_.d		= 2 + i * 4;
		l_.d_upper = 4 + i * 4;
		road->L.push_back(l_);
	}

	State->lane_change_end_time = chrono::high_resolution_clock::now() - 5000ms;

}


Behavior::lane Behavior::update_behavior_state(map<int, Vehicle> other_vehicles) {

	// check if in current maneuver
	if (State->lane_change_end_time > chrono::high_resolution_clock::now()) 
	{
		return State->L_target;  // this could be expanded considerably but for now just locking out new moves while moving
	}
	else {

		update_lane_costs(other_vehicles);
		road->L[2].cost = 0;
		road->L[1].cost = 10;

		find_best_lane();
		State->lane_change_end_time = chrono::high_resolution_clock::now() + 5000ms;

		return State->L_target;
	}
}


void Behavior::update_lane_costs(map<int, Vehicle> other_vehicles) {

	// Assign costs to lanes based on sensor data
	for (size_t i = 0; i < road->L.size(); ++i) {

		// TODO for now just assigns loop cost
		road->L[i].cost = i;
	}
	road->L[1].cost = 0;
	road->L[0].cost = 10;
	
}


void update_behavior_targets() {


}

void Behavior::find_best_lane() {

	float cost = 1e9;
	for (size_t i = 0; i < road->L.size(); ++i) {
		if (road->L[i].cost < cost) { 
			State->L_target = road->L[i];
			cost = road->L[i].cost;
		}
	}
}





