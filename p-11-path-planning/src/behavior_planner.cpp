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
		l_.d_lower = i * 1;
		l_.d		= i * 2;
		l_.d_upper = i * 3;
		road->L.push_back(l_);
	}

}

void Behavior::update_lanes(map<int, Vehicle> other_vehicles) {
	
	// Assign costs to lanes based on sensor data
	for (size_t i = 0; i < road->L.size(); ++i) {

		road->L[i].cost = i;

	}
	
}


int Behavior::update_behavior_state() {

	// check if in current maneuver
	if (State->lane_change_end_time > chrono::high_resolution_clock::now()) 
	{
		return 1;  // this could be expanded considerably but for now just locking out new moves while moving
	}
	else {
		update_lane();
	}
}


void Behavior::update_lane() {

	// Update target based on behavior state
	float cost = 1e9;
	lane best_lane;
	for (size_t i = 0; i < road->L.size(); ++i) {

		road->L[i].cost = i;

	}

	// Choose lane with minimum cost based on update lanes

	// Check if valid move

}





