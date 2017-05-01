//=================================================================================
// Name        : 1d_bayesian filter
// Version     : 1.0.0
// Copyright   : MBRDNA, Udacity
//=================================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "measurement_package.h"
#include "map.h"
#include "help_functions.h"

using namespace std;

int main() {

	/******************************************************************************
	 *  declaration:															      
	 *****************************************************************************/
	
	//define example: 01, 02, 03, 04
	string example_string = "01";

	//declare map:
	map map_1d;

	//declare measurement list:
	std::vector<MeasurementPackage> measurement_pack_list;

	//declare helpers:
	help_functions helper;

	//define file names:
	char in_file_name_ctr[1024];
	char in_file_name_obs[1024];
	char in_file_name_gt[1024];

	/******************************************************************************
	 *  read map and measurements:											     *
	 *****************************************************************************/
	//read map:
	helper.read_map_data("data/map_1d.txt", map_1d);

	//define file name of controls:
	sprintf(in_file_name_ctr, "data/example%s/control_data.txt", 
			example_string.c_str());

	//define file name of observations:
	sprintf(in_file_name_obs, "data/example%s/observations/", 
			example_string.c_str());
	
	//read in data to measurement package list:
	helper.read_measurement_data(in_file_name_ctr, 
			                     in_file_name_obs, 
			                     measurement_pack_list);

	/*****************************************************************************
		 *  Coding quiz 1: just print out map infos and measurement package:     *
    ******************************************************************************/

	cout << "Landmarks" << endl;

	for(int i = 0;		i < map_1d.landmark_list.size();	i++){

		cout << "ID"  << map_1d.landmark_list[i].id_i << "\t"
			 << "X: " << map_1d.landmark_list[i].x_f  << endl;
	}

	cout << endl << ".-|-.-|-.-|-.-|-.-|-.-|-.-|-.-|-.-|-.-|-." << endl << endl;


	cout << "Measurement package:" << endl;

	int observation_size = 0;

	for(int i = 0;		i < measurement_pack_list.size();		i++){

		cout << "Step " << i << "\t" << "-> " << measurement_pack_list[i].control_s_.delta_x_f
			 << " meters in driving direction " << endl;

		// Observations
		observation_size = measurement_pack_list[i].observation_s_.distance_f.size();

		if (observation_size < 1){
			cout << "No Observations. Observations size: " << observation_size << endl << endl;
		}
		else {

			cout << "Valid Observations " << observation_size << endl;

			for(int j = 0;		j < observation_size;		j++){

				cout << "Distance to landmark: " 
				<< measurement_pack_list[i].observation_s_.distance_f[j]
				<< " meters" << endl << endl;
			}
		}
	}

	return 0;
}