//============================================================================
// Name        : bayesianFilter.cpp
// Version     : 1.0.0
// Copyright   : MBRDNA, Udacity
//============================================================================

#include "bayesianFilter.h"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

//constructor:
bayesianFilter::bayesianFilter() {


	is_initialized_ = false;

	//standard deviation of control
	control_std = 1.0f;

	//define size of believe, same size as map
	bel_x.resize(100,0);
	
	// NOTE: helps us not overwrite believe during
	// the motion calculation
	bel_x_init.resize(100,0);

}

//de-constructor:
bayesianFilter::~bayesianFilter() {

}

void bayesianFilter::process_measurement(const MeasurementPackage &measurements,
        						             const map &map_1d,
                                         help_functions &helpers){

	/******************************************************************************
	 *  Set init belief of state vector:
	 ******************************************************************************/
	 if(!is_initialized_){

		//Run over map, all map_1d.lanmark_list values:
		for (int l = 0 ;	l < map_1d.landmark_list.size();		l++){

			// Define landmark_omega
			map::single_landmark_s		landmark_omega;
			landmark_omega = map_1d.landmark_list[l];

			//check, if landmark position x fits in map [0,100]:
			if(landmark_omega.x_f < bel_x_init.size() &&  
			   landmark_omega.x_f > 0			 ){

				// Get landmark x position * use help_function.h for reference
				int position_x = int(landmark_omega.x_f) ;
				
				//Set belief to 1 at position and +/- from position:
				bel_x_init[position_x - 1] = 1.0f;
				bel_x_init[position_x    ] = 1.0f;
				bel_x_init[position_x + 1] = 1.0f;
			}
		}

		// Normalize initial belief
		bel_x_init = helpers.normalize_vector(bel_x_init)	;

		is_initialized_ = true	;
	}
	
	/******************************************************************************
	 *  motion model and observation update
	******************************************************************************/
	std::cout <<"-->motion model for state x ! \n" << std::endl	;

	//get current observations and control information:
	MeasurementPackage::control_s     controls = measurements.control_s_;	// mu
	MeasurementPackage::observation_s observations = measurements.observation_s_;

	int belief_size = bel_x.size();

	//run over all bel_x values (index represents the pose in x!):
	for (int i = 0; 	i < belief_size; 	i++){

		/**************************************************************************
		 *  posterior for motion model
		**************************************************************************/

		float posterior_motion = 0.0f;  // place holder to be overwritten
		float position_initial = float(i);

		// State space x_t-1 * same size as bel_x (Perform Convolution):
		for (int j = 0;	 	j < belief_size;		j++) {
			
			// Difference between bel_x index and state space index
			float position_j = float(j) ;
			float x_distance_i_j = position_initial - position_j;

			// Calculate transition probabilites
			float transition_probability = helpers.normpdf( x_distance_i_j,
													 		controls.delta_x_f,
													 		control_std);

			// Motion model, Add the transition prob multiplied by the intial believe 
			// at state space index
			posterior_motion += transition_probability * bel_x_init[ j];
		}

		// Update  motion_model
		bel_x[i] = posterior_motion;

	};
		// Normalize bel_x:
		bel_x = helpers.normalize_vector(bel_x);
		
		// Set initial believe to bel_x for next time
		bel_x_init = bel_x; 

};