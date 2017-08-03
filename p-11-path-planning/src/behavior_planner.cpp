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
		l_.d_lower = i *  2.5;
		l_.d		= 3.9 + i * 3;
		l_.d_upper = 3.9 + i * 3;
		road->L.push_back(l_);
	}

	State->lane_change_end_time = chrono::high_resolution_clock::now() - 10000ms;

	State->L.id = 1;
}


Behavior::lane Behavior::update_behavior_state(vector<double> trajectory, path *our_path) {

	// check if in current maneuver
	if (State->lane_change_end_time > chrono::high_resolution_clock::now()) 
	{
		return State->L_target;  // this could be expanded considerably but for now just locking out new moves while moving
	}
	else {

		update_lane_costs(trajectory, our_path);
		find_best_lane();
		
		// only update clock if state changes
		if (State->L_target.id != State->L.id) {
			State->lane_change_end_time = chrono::high_resolution_clock::now() + 10000ms;
			State->L.id = State->L_target.id;
			cout << "Changing lanes" << endl;
		}

		return State->L_target;
	}
}


void Behavior::update_lane_costs(vector<double> trajectory, path *our_path) {

	// Assign costs to lanes based on sensor data
	vector<double> t_0, t_1, t_2;
	t_0 = trajectory;
	t_1 = trajectory;
	t_2 = trajectory;

	t_0[6] -= 4;
	t_2[6] += 4;

	road->L[0].cost += our_path->collision_cost(t_0);
	road->L[0].cost += our_path->buffer_cost(t_0);
	//road->L[0].cost += our_path->total_acceleration_cost(t_0);

	road->L[1].cost += our_path->collision_cost(t_1);
	road->L[1].cost += our_path->buffer_cost(t_1);
	//road->L[1].cost += our_path->total_acceleration_cost(t_1);

	road->L[2].cost += our_path->collision_cost(t_2);
	road->L[2].cost += our_path->buffer_cost(t_2);
	//road->L[2].cost += our_path->total_acceleration_cost(t_2);

	cout << road->L[0].cost << " " << road->L[1].cost << " " << road->L[0].cost << endl;
	
}


void update_behavior_targets() {


}

void Behavior::find_best_lane() {

	double cost = 1e9;
	
	for (size_t i = 0; i < road->L.size(); ++i) {
		if (road->L[i].cost < cost) { 
			State->L_target = road->L[i];
			cost = road->L[i].cost;
		}
	}
}





