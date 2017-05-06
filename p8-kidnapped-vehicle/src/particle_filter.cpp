#include <random>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <map>

#include "particle_filter.h"

using namespace std;


void ParticleFilter::init(double x, double y, double theta, double std[]) {
	// Initialize all particles to first position (based on estimates of 
	//   x, y, theta and their uncertainties from GPS)

	num_particles = 10;

	random_device rd;
    mt19937 gen(rd());
	// default_random_engine gen;

	normal_distribution<double> dist_x(		x, 		std[0]) ;
	normal_distribution<double> dist_y(		y, 		std[1]) ;
	normal_distribution<double> dist_psi(	theta, 	std[2]);

	weights.resize(num_particles) ;
	particles.resize(num_particles) ;

	for (int i = 0; i < num_particles; i++) {
		
		particles[i].id 	= i ;
		particles[i].x 		= dist_x(gen) ;  // Add random Gaussian noise to each particle.
		particles[i].y 		= dist_y(gen) ;
		particles[i].theta 	= dist_psi(gen) ;
		particles[i].weight = 1.0 ;
		// cout << "Sample " << i + 1 << " " << sample_x << " " << sample_y << " " << sample_psi << endl;
	}

	is_initialized = true;
	cout << "Initialization complete" << endl;
}


void ParticleFilter::prediction(double delta_t, double std_pos[], double velocity, double yaw_rate) {

	// cout << "start prediction" << endl;
	random_device rd;
    mt19937 gen(rd());

	for (int i = 0; 	i < num_particles; 		i++) {
		
		const double theta = particles[i].theta ;

		// cout << "Prediction step:" << i << endl;

		// Add measurements

		if ( fabs(yaw_rate) > 0.00001) {		// Turning

			const double velocity_yaw_rate = velocity / yaw_rate ;
			const double yaw_rate_delta_t  = yaw_rate * delta_t ;

			particles[i].x 		+=  velocity_yaw_rate * (sin(theta + yaw_rate_delta_t) - sin(theta)) ;
			particles[i].y 		+=  velocity_yaw_rate * (cos(theta) - cos(theta + yaw_rate_delta_t)) ;
			particles[i].theta 	+= yaw_rate * delta_t ;
		}

		else {		// Going straight
			
			const double velocity_delta_t = velocity * delta_t ;

			particles[i].x += velocity_delta_t * cos(theta) ;
			particles[i].y += velocity_delta_t * sin(theta) ;
			// theta excluded as going straight
		}

		// add random Gaussian noise
		normal_distribution<double> dist_x(		particles[i].x, 	std_pos[0]) ;
		normal_distribution<double> dist_y(		particles[i].y, 	std_pos[1]) ;
		normal_distribution<double> dist_psi(	particles[i].theta, 	std_pos[2]);

		particles[i].x 		= dist_x(	gen) ;  // += since normal_distribution center on 0.
		particles[i].y 		= dist_y(	gen) ;
		particles[i].theta 	= dist_psi(gen) ;
	}
	// cout << "prediction complete" << endl;
}


vector<int> ParticleFilter::dataAssociation( std::vector<LandmarkObs> reasonable_landmarks, 
									        std::vector<LandmarkObs>& transformed_observations) {

	// 3. Associate each predicted measurement to it's closest point
	// cout << reasonable_landmarks.size() << "\t" << transformed_observations.size() << endl;
	// loop through all observations
	vector<int> transformed_observations_landmark_ids ;
	transformed_observations_landmark_ids.resize(transformed_observations.size()) ;

	for (unsigned int i = 0;	i < transformed_observations.size();	 i++){

		double threshold = 9999999.0 ;
		int current_id ;

		// for each observation, compare it to all reasonable landmarks
		for (unsigned int j = 0;	j < reasonable_landmarks.size();	 j++){

		// Find the predicted measurement that is closest to each observed measurement
		double associate_difference = dist( reasonable_landmarks[j].x,    reasonable_landmarks[j].y, 
						   transformed_observations[i].x, transformed_observations[i].y) ;
			
			// check if threshold is less than previous
			if (associate_difference < threshold) {
				
				// if it is, then we have the current lowest ID
				current_id = reasonable_landmarks[j].id ;

				// assign the threshold to the current difference
				// so that next loop if the difference is higher it won't update the id.
				threshold = associate_difference ;
			}
		}
		// assign the id of the correct particle from the observation to a list	
		transformed_observations_landmark_ids[i] = current_id ;
		// cout << current_id << endl ;
		// cout << reasonable_particles_landmark_ids[i] << endl ;
	}

	// cout << "difference \t" << difference << endl ;
	// cout << "reasonable_particles_landmark_ids size: \t" << reasonable_particles_landmark_ids.size()
	// << "\t" << reasonable_landmarks.size() << endl;
	// cout << "dataAssociation complete, reasonable landmarks size: " << reasonable_landmarks.size() << endl;

	return transformed_observations_landmark_ids ;
}


vector<LandmarkObs> ParticleFilter::transformed_observations( Particle				particle_, 
															  vector<LandmarkObs>   observations){

	// Create placeholder for transformations
	vector<LandmarkObs> transformed_observations ;
	LandmarkObs 		single_transformed_observation ;

	const int observations_size = observations.size() ;
	transformed_observations.resize(observations_size) ;

	for (int k = 0; 	k < observations_size;		k++) {

		// Pre-compute terms
		const double particle_theta = particle_.theta ;
		const double cos_theta 		= cos(particle_theta) ;
		const double sin_theta 		= sin(particle_theta) ; 

		// p_x + (obs_x * cos(theta)) - (obs_y * sin(theta))
		single_transformed_observation.x =  particle_.x    + 
										   (observations[k].x * cos_theta) - 
										   (observations[k].y * sin_theta) ;

		// p_y + (obs_x * sin(theta)) + (obs_y * cos(theta))					   
		single_transformed_observation.y =  particle_.y    +
										   (observations[k].x * sin_theta) + 
										   (observations[k].y * cos_theta) ;

		transformed_observations[k] = single_transformed_observation ;
	}

	return transformed_observations ;

}


vector<LandmarkObs> ParticleFilter::reasonable_landmarks( vector<LandmarkObs>   transformed_observations,
																   		Map		map_landmarks,
															  	   	 double 	sensor_range){
	vector<LandmarkObs> reasonable_landmarks ;
	int landmark_limit = map_landmarks.landmark_list.size() ;
	reasonable_landmarks.resize(transformed_observations.size()) ;  // for debugging

	vector<int> reasonable_particles_landmark_ids ;
	reasonable_particles_landmark_ids.resize(transformed_observations.size()) ;

	for (unsigned int i = 0;	i < transformed_observations.size();	 i++) {
		
		for (int q = 0;		q < landmark_limit;		q++){

			const double landmark_position_x = map_landmarks.landmark_list[q].x_f ;
			const double landmark_position_y = map_landmarks.landmark_list[q].y_f ;
			
			// 2.1 Compare particle distance to landmark distance
			double difference = dist( transformed_observations[i].x, 
						   	          transformed_observations[i].y,
						   	          landmark_position_x, 
						   	          landmark_position_y);
			// cout << difference << "  " << sensor_range << endl; 
			// Only look at landmarks within sensor range
			if (difference <= sensor_range) {

				const int landmark_id = map_landmarks.landmark_list[q].id_i - 1 ;
				// cout << "landmark_id\t" << landmark_id << endl ;

				reasonable_landmarks.push_back( LandmarkObs { landmark_id, 
															  landmark_position_x,
															  landmark_position_y }) ;
			}
		}
	}

	return reasonable_landmarks ;

	/*
	for (int running_out_of_ints = 0;	running_out_of_ints << reasonable_landmarks.size(); running_out_of_ints++){

		cout << "reasonable_landmarks" << "\t" << reasonable_landmarks[running_out_of_ints].x 
		<< "," << running_out_of_ints[ggg].y  << endl ;

	}
	*/
}

void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
		std::vector<LandmarkObs> observations, Map map_landmarks) {


	weights.clear();

	// cout << "Starting update weights" << endl;
	for (int i = 0; 	i < num_particles; 		i++) {

		// key note, all of this is for 1 particle, ie a particle easily may have 5+ obersvations
		// 1. Convert particle observations to global frame
		vector<LandmarkObs> transformed_observations ;
		transformed_observations = ParticleFilter::transformed_observations(particles[i], observations) ;
		// cout << transformed_observations.size() << endl;

		// 2. Only keep landmarks that are within range
		// reasonable_landmarks 	= landmarks obverserved by particles, that are also within sensor range
		// vector<LandmarkObs> reasonable_landmarks ;
		// reasonable_landmarks = ParticleFilter::reasonable_landmarks(transformed_observations, map_landmarks, sensor_range) ;
		// 3. Create list of landmark ids associated with observations
		// vector<int> transformed_observations_landmark_ids;
		//transformed_observations_landmark_ids = ParticleFilter::dataAssociation(map_landmarks, transformed_observations) ;

		// 4. Weight distribution
		const double standard_x 			= std_landmark[0] ;
		const double standard_y 			= std_landmark[1] ;
		const double standard_x_squared_2	= (standard_x * standard_x) ;
		const double standard_y_squared_2	= (standard_y * standard_y) ;
		const double part_1 				= 1 / (2 * M_PI * standard_x * standard_y) ;
		// cout << part_1 << endl;
		//cout << reasonable_landmarks.size() << "\t" << transformed_observations.size() << endl;

		double weight_placeholder = 1.0 ;
		// if reasonable landmarks have been found
			// cout << reasonable_particles_landmark_ids.size() << "\t" << transformed_observations.size() << endl;
		for (unsigned int j = 0; 	j < transformed_observations.size();	j++) {

			double threshold = 9999999.0 ;
			int current_id = -2;
			for (unsigned int q = 0; 	q < map_landmarks.landmark_list.size();	q++) {
				// Find the predicted measurement that is closest to each observed measurement
				double associate_difference = dist( map_landmarks.landmark_list[q].x_f,    map_landmarks.landmark_list[q].y_f, 
								   transformed_observations[j].x, transformed_observations[j].y) ;
				// check if threshold is less than previous
				if (associate_difference < threshold) {
					// if it is, then we have the current lowest ID
					current_id = map_landmarks.landmark_list[q].id_i ;
					// assign the threshold to the current difference so that next loop if the difference is higher it won't update the id.
					threshold = associate_difference ;
				}
				if (current_id >= 0) {

					const long double x 				= transformed_observations[q].x ;
					const long double u_x 				= map_landmarks.landmark_list[current_id].x_f ;
					const long double y 			  	= transformed_observations[q].y ;
					const long double u_y 		  		= map_landmarks.landmark_list[current_id].y_f ;

					const long double x_ux_squared 		= (x - u_x) * (x - u_x) ;
					const long double y_uy_squared 		= (y - u_y) * (y - u_y) ;
					const long double part_2 			= x_ux_squared / (standard_x_squared_2) +
										 		  		  y_uy_squared / (standard_y_squared_2) ;
					long double exp_part_2				= exp( (-1/2) * (part_2)) ;

					// cout << "transformed_observations\t" << x << "," << y <<
					// "\t reasonable_landmarks\t" << u_x << "," << u_y << endl ;
					// cout << weight_placeholder << endl;
					// cout << part_2 << "\t" << exp_part_2 << endl;

					weight_placeholder *= part_1 * exp_part_2 ;
					cout << weight_placeholder << endl;
					// cout << "x " << x << "\tu_x " << u_x << "\ty " << y << "\tu_y " <<  u_y << endl ;
				}
			}
		}
		//weights[i] 			= weight_placeholder ;
		particles[i].weight = weight_placeholder ;
		weights.push_back(particles[i].weight);
		// cout << "Weight update complete" << endl;

		//cout << particles[i].x << "," << particles[i].y << ","  << particles[i].theta << endl;
	}
}

void ParticleFilter::resample() {
	// Resample particles with replacement with probability proportional to their weight.
	// cout << "Resample started " << " " << particles.size() << endl;

	// from http://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
    random_device rd;
    mt19937 gen(rd());
	discrete_distribution<> d(weights.begin(), weights.end());

	// cout << "Number of particles \t" << num_particles << endl;

	vector<Particle> resampled_particles ;
	resampled_particles.resize(num_particles) ;

    for(int n = 0;	n < num_particles;	n++) {

    	// cout << "Resample step: \t" << n << endl;
    	int new_index = d(gen);
    	// cout << new_index << endl ;
        resampled_particles[n] = particles[new_index];
    }
    // cout << "Resample complete" << endl;
    particles = resampled_particles ;
}

void ParticleFilter::write(std::string filename) {
	// You don't need to modify this file.
	std::ofstream dataFile;
	dataFile.open(filename, std::ios::app);
	for (int i = 0; i < num_particles; ++i) {
		dataFile << particles[i].x << " " << particles[i].y << " " << particles[i].theta << "\n";
	}
	dataFile.close();
}
