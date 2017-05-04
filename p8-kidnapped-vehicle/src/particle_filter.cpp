/*
 *  5/3/17
 *  Student author: Anthony Sarkis
 */

#include <random>
#include <algorithm>
#include <iostream>
#include <numeric>

#include "particle_filter.h"

using namespace std;


void ParticleFilter::init(double x, double y, double theta, double std[]) {
	// Set the number of particles. Initialize all particles to first position (based on estimates of 
	//   x, y, theta and their uncertainties from GPS) and all weights to 1. 
	// Add random Gaussian noise to each particle.
	// NOTE: Consult particle_filter.h for more information about this method (and others in this file).

	num_particles = 100;

	default_random_engine gen;

	normal_distribution<double> dist_x(x, std[0]) ;
	normal_distribution<double> dist_y(y, std[1]) ;
	normal_distribution<double> dist_psi(theta, std[2]);

	for (int i = 0; i < num_particles; ++i) {
		
		particles[i].x = dist_x(gen) ;
		particles[i].y = dist_y(gen) ;
		particles[i].theta = dist_psi(gen) ;
		particles[i].weight = 1 ;
		
		// cout << "Sample " << i + 1 << " " << sample_x << " " << sample_y << " " << sample_psi << endl;
	}

	is_initialized = true;
}

void ParticleFilter::prediction(double delta_t, double std_pos[], double velocity, double yaw_rate) {
	// Add measurements to each particle and add random Gaussian noise.
	// NOTE: When adding noise you may find std::normal_distribution and std::default_random_engine useful.
	//  http://en.cppreference.com/w/cpp/numeric/random/normal_distribution
	//  http://www.cplusplus.com/reference/random/default_random_engine/
	
	default_random_engine gen;

	normal_distribution<double> dist_x(0, std_pos[0]) ;
	normal_distribution<double> dist_y(0, std_pos[1]) ;
	normal_distribution<double> dist_psi(0, std_pos[2]);

	for (int i = 0; i < num_particles; ++i) {
		
		particles[i].x += dist_x(gen) ;
		particles[i].y += dist_y(gen) ;
		particles[i].theta += dist_psi(gen) ;

		if ( yaw_rate > 0) { 
			particles[i].x += velocity / yaw_rate * (sin(particles[i].theta + yaw_rate*(delta_t)) - sin(particles[i].theta)) ;
			particles[i].y += velocity / yaw_rate * (cos(particles[i].theta) - cos(particles[i].theta + yaw_rate*(delta_t))) ;
			particles[i].theta += yaw_rate*delta_t ;
		}
		// cout << "Sample " << i + 1 << " " << sample_x << " " << sample_y << " " << sample_psi << endl;
	}
}

void ParticleFilter::dataAssociation(std::vector<LandmarkObs> predicted, std::vector<LandmarkObs>& observations) {
	// TODO: Find the predicted measurement that is closest to each observed measurement and assign the 
	//   observed measurement to this particular landmark.
	// NOTE: this method will NOT be called by the grading code. But you will probably find it useful to 
	//   implement this method and use it as a helper during the updateWeights phase.

}

void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
		std::vector<LandmarkObs> observations, Map map_landmarks) {
	// TODO: Update the weights of each particle using a mult-variate Gaussian distribution. You can read
	//   more about this distribution here: https://en.wikipedia.org/wiki/Multivariate_normal_distribution
	// NOTE: The observations are given in the VEHICLE'S coordinate system. Your particles are located
	//   according to the MAP'S coordinate system. You will need to transform between the two systems.
	//   Keep in mind that this transformation requires both rotation AND translation (but no scaling).
	//   The following is a good resource for the theory:
	//   https://www.willamette.edu/~gorr/classes/GeneralGraphics/Transforms/transforms2d.htm
	//   and the following is a good resource for the actual equation to implement (look at equation 
	//   3.33. Note that you'll need to switch the minus sign in that equation to a plus to account 
	//   for the fact that the map's y-axis actually points downwards.)
	//   http://planning.cs.uiuc.edu/node99.html


	for (int i = 0; i < num_particles; ++i) {
		
		particles[i].x ;
		particles[i].y ;
		particles[i].theta  ;

	}

}

void ParticleFilter::resample() {
	// TODO: Resample particles with replacement with probability proportional to their weight. 
	// NOTE: You may find std::discrete_distribution helpful here.
	//   http://en.cppreference.com/w/cpp/numeric/random/discrete_distribution

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
