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

int previous_id = 1;

void Behavior::init() {

	for (size_t i = 0; i < 3; ++i) {
		lane l_;
		l_.id = i;
		//l_.d_lower = i *  1;
		//l_.d_upper = 4 + i * 1;
		road->L.push_back(l_);
	}

	road->L[0].d = 6 - 3.8;
	road->L[1].d = 6;
	road->L[2].d = 6 + 3.8;

	State->lane_change_end_time = chrono::high_resolution_clock::now() + 30000ms;
	State->L_target = road->L[1];
}


Behavior::lane Behavior::update_behavior_state(vector<double> trajectory, path *our_path) {

	// check if in current maneuver
	// cout << chrono::high_resolution_clock::to_time_t(State->lane_change_end_time) << endl;
	// cout << chrono::high_resolution_clock::to_time_t(chrono::high_resolution_clock::now()) << endl;

	if (State->lane_change_end_time > chrono::high_resolution_clock::now() || 
		our_path->ref_velocity < 30 || our_path->ref_velocity > 45)
	{
		return State->L_target;  // this could be expanded considerably but for now just locking out new moves while moving
	}
	else {

		update_lane_costs(trajectory, our_path);
		find_best_lane();
		
		// only update clock if state changes
		if (previous_id != State->L_target.id) {
			
			State->lane_change_end_time = chrono::high_resolution_clock::now() + 5000ms;
			previous_id = State->L_target.id;
			cout << "\n*** Changing lanes \t" << State->L_target.d << "\n" <<endl;
			//cout << "L target " << State->L_target.id << endl;
		}
		//cout << "\n In lowest cost lane \t" << endl;

		return State->L_target;
	}
}


void Behavior::update_lane_costs(vector<double> trajectory, path *our_path) {

	// Assign costs to lanes based on sensor data
	vector<double> t_0, t_1, t_2;
	t_0 = trajectory;
	t_1 = trajectory;
	t_2 = trajectory;

	if (previous_id == 0) {
		t_1[6] = road->L[1].d;
		t_2[6] = road->L[1].d;
	}
	if (previous_id == 1) {
		t_0[6] = road->L[0].d;
		t_2[6] = road->L[2].d;;
	}
	if (previous_id == 2) {
		t_0[6] = road->L[1].d;;
		t_1[6] = road->L[1].d;;
	}

	
	road->L[0].cost += our_path->buffer_cost_front(t_0);
	road->L[0].cost += 2 * our_path->buffer_cost(t_0);
	road->L[0].cost += .5 * our_path->total_acceleration_cost(t_0);
	road->L[0].cost += .5 * our_path->max_acceleration_cost(t_0);
	//road->L[0].cost += 1 * our_path->collision_cost(t_0);

	road->L[1].cost += our_path->buffer_cost_front(t_1);
	road->L[1].cost += 2 * our_path->buffer_cost(t_1);
	road->L[1].cost += .5 * our_path->total_acceleration_cost(t_1);
	road->L[1].cost += .5 * our_path->max_acceleration_cost(t_1);
	//road->L[1].cost += 1 * our_path->collision_cost(t_1);

	road->L[2].cost += our_path->buffer_cost_front(t_2);
	road->L[2].cost +=  2 * our_path->buffer_cost(t_2);
	road->L[2].cost += .5 * our_path->total_acceleration_cost(t_2);
	road->L[2].cost += .5 * our_path->max_acceleration_cost(t_2);
	//road->L[2].cost += 1 * our_path->collision_cost(t_2);

	
	cout << "Lane0 " << road->L[0].cost << "\tL1 " << road->L[1].cost << "\tL2 " << road->L[2].cost << endl;
	
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

	// reset cost
	for (size_t i = 0; i < road->L.size(); ++i){
		road->L[i].cost = 0;
	}
}





