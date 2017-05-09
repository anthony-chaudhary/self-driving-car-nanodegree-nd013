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

	num_particles = 300;

	random_device rd;
    mt19937 gen(rd());
	// default_random_engine gen;

	normal_distribution<double> dist_x(		x, 		std[0]) ;
	normal_distribution<double> dist_y(		y, 		std[1]) ;
	normal_distribution<double> dist_psi(	theta, 	std[2]);

	// weights.resize(num_particles) ;
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


void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
    std::vector<LandmarkObs> observations, Map map_landmarks) {


  weights.clear();

  // cout << "Starting update weights" << endl;
  for (int i = 0;   i < num_particles;    i++) {

    // key note, all of this is for 1 particle, ie a particle easily may have 5+ obersvations
    // 1. Convert particle observations to global frame
    // cout << transformed_observations.size() << endl;

    // 2. Only keep landmarks that are within range
    // reasonable_landmarks   = landmarks obverserved by particles, that are also within sensor range
    // vector<LandmarkObs> reasonable_landmarks ;
    // reasonable_landmarks = ParticleFilter::reasonable_landmarks(transformed_observations, map_landmarks, sensor_range) ;
    // 3. Create list of landmark ids associated with observations
    // vector<int> transformed_observations_landmark_ids;
    //transformed_observations_landmark_ids = ParticleFilter::dataAssociation(map_landmarks, transformed_observations) ;

    // 4. Weight distribution
    const double standard_x       = std_landmark[0] ;
    const double standard_y       = std_landmark[1] ;
    const double standard_x_squared_2 = (standard_x * standard_x) ;
    const double standard_y_squared_2 = (standard_y * standard_y) ;
    const long double part_1      = 1 / (2 * M_PI * standard_x * standard_y) ;
    // cout << part_1 << endl;
    //cout << reasonable_landmarks.size() << "\t" << transformed_observations.size() << endl;

    long double weight_placeholder = 1.0 ;
    // if reasonable landmarks have been found
    // cout << reasonable_particles_landmark_ids.size() << "\t" << transformed_observations.size() << endl;
    for (unsigned int j = 0;  j < observations.size();  j++) {
  
      const double particle_theta = particles[i].theta ;
      const double cos_theta    = cos(particle_theta) ;
      const double sin_theta    = sin(particle_theta) ; 
      // p_x + (obs_x * cos(theta)) - (obs_y * sin(theta))
      double transformed_observation_x =  particles[i].x   + (observations[j].x * cos_theta) - 
                          (observations[j].y * sin_theta) ;
      // p_y + (obs_x * sin(theta)) + (obs_y * cos(theta))             
      double transformed_observation_y =  particles[i].y   + (observations[j].x * sin_theta) + 
                         (observations[j].y * cos_theta) ;
      double threshold = 50.0 ;
      long current_id = -1;

      for (unsigned int q = 0;  q < map_landmarks.landmark_list.size(); q++) {

        // Find the predicted measurement that is closest to each observed measurement
        double difference = dist( transformed_observation_x, transformed_observation_y,
          map_landmarks.landmark_list[q].x_f,    map_landmarks.landmark_list[q].y_f ) ;
        // check if threshold is less than previous
        if (difference < threshold) {
          // if it is, then we have the current lowest ID
          // map data is not zero indexed
          current_id = map_landmarks.landmark_list[q].id_i -1 ;
          // assign the threshold to the current difference so that next loop if the difference is higher it won't update the id.
          threshold = difference ;
          }
      }

      // cout << current_id << endl ;

      if (current_id >= 0) {

        const long double x         = transformed_observation_x ;
        const long double u_x         = map_landmarks.landmark_list[current_id].x_f ;
        const long double y           = transformed_observation_y ;
        const long double u_y           = map_landmarks.landmark_list[current_id].y_f ;
        const long double x_ux_squared    = (x - u_x) * (x - u_x);
        const long double y_uy_squared    = (y - u_y) * (y - u_y);
        const long double part_2      = x_ux_squared / (standard_x_squared_2) +
                              y_uy_squared / (standard_y_squared_2) ;
        // long double exp_part_2        = exp( (-1/2) * (part_2)) ;  // ERROR

        // cout << "transformed_observations\t" << x << "," << y << endl ;
        // "\t reasonable_landmarks\t" << u_x << "," << u_y << endl ;
        // cout << weight_placeholder << endl;
        // cout << part_2 << "\t" << exp_part_2 << endl;

        // long double w_i = part_1 * exp( (-1/2.) * (part_2)) ;
        // weight_placeholder *= w_i ;

        weight_placeholder *= part_1 * exp( (-1/2.) * (part_2));

        // cout << weight_placeholder << endl;
        // cout << "x " << x << "\tu_x " << u_x << "\ty " << y << "\tu_y " <<  u_y << endl ;
      }
    }
    
    //weights[i]      = weight_placeholder ;
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
