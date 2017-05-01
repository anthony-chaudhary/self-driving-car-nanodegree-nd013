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

    //TODO add is_initialized to header file
    //set initialization to false:
    // NOTE: helps us set up the initial believe state
	//is_initialized_ = 

    //TODO add control_std to header file
	//set standard deviation of control to 1.0f:
	//control_std = 

	//define size of believe, same size as map
	bel_x.resize(100,0);
	
	//TODO add bel_x_init to header file
	// NOTE: helps us not overwrite believe during
	// the motion calculation
	//bel_x_init

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
	 //if(!is_initialized_){

		//TODO: run over map, all map_1d.lanmark_list values:
		//for (int l=0; l< ...){

			//TODO: get landmark l from map 
			

			//check, if landmark position x fits in map [0,100]:
			//if(... ){

				//TODO: get landmark x position * use help_function.h for reference
				
				// ______/---\_____ << initial believe state at a landmark
				
				//TODO: set belief to 1 at position and +/- from position:
				

		//	} //end if
		//}//end for

	//TODO: normalize initial believe * use help_function.h for reference
	//bel_x_init = 

	//set initial flag to true:
	//is_initialized_ = 
	
	//}//end if
	
	/******************************************************************************
	 *  motion model and observation update
	******************************************************************************/
	std::cout <<"-->motion model for state x ! \n" << std::endl;

	//get current observations and control information:
	MeasurementPackage::control_s     controls = measurements.control_s_;
	MeasurementPackage::observation_s observations = measurements.observation_s_;

	//run over all bel_x values (index represents the pose in x!):
	//for (int i=0; i< ...){


		/**************************************************************************
		 *  posterior for motion model
		**************************************************************************/

        // motion posterior:
        // used to set up the convlution
		float posterior_motion = 0.0f;

		//loop over state space x_t-1 * same size as bel_x (Perform Convolution):
		//for (int j=0; j< ...){
			
			//TODO: Calculate transition probabilites using helpers.normpdf()
			// x: difference between bel_x index and state space index
			// mu: the movement from controls defined above
			// std: defined eariler
			
			//TODO: Calculate motion model
			// ADD the transition prob multiplied by the intial believe 
			// at state space index
			//posterior_motion +=
		//}

		//TODO: update = motion_model 
		// set as the posterior_motion
		//bel_x[i] =


	//};
		//TODO: normalize bel_x:
		

		//TODO: set initial believe to bel_x for next time
		//bel_x_init = 
	 
	 
	 

};