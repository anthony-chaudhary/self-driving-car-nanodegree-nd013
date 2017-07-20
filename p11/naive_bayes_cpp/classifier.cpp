#include <iostream>
#include <sstream>
#include <fstream>


#include <math.h>
#include <vector>
#include "classifier.h"

/**
* Initializes GNB
*/
GNB::GNB() {

}

GNB::~GNB() {}

void GNB::train(vector<vector<double>> data, vector<string> labels)
{

	/*
	Trains the classifier with N data points and labels.

	INPUTS
	data - array of N observations
	- Each observation is a tuple with 4 values: s, d,
	s_dot and d_dot.
	- Example : [
	[3.5, 0.1, 5.9, -0.02],
	[8.0, -0.3, 3.0, 2.2],
	...
	]

	labels - array of N labels
	- Each label is one of "left", "keep", or "right".
	*/

	//
	// int n_features = data.size();
	// int n_classes = possible_labels.size();
	// theta_ = matrix()

	double len_labels = labels.size();
	cout << "\nLen_labels " << len_labels << endl;

	// chance of being left
	int left_count = 0;
	double left_data_sum = 0;
	vector<double> left_data_variance;

	for (size_t i = 0; i < len_labels; ++i) {
		if (labels[i] == "left") {
			left_count += 1;
			left_data_sum += data[i][0];
			left_data_variance.push_back(data[i][0]);
		}
	}

	double left_chance = left_count / len_labels;
	double left_data_mean = left_data_sum / left_count;

	double left_data_variance_sum = 0;
	for (size_t i = 0; i < left_data_variance.size(); ++i) {
		double difference = left_data_mean - left_data_variance[i];
		double square = pow(difference, 2);
		left_data_variance_sum += square;
	}
	double left_data_variance_mean = left_data_variance_sum / left_count;

	cout << "Left chance " << left_chance << " left_count " << left_count << endl;
	cout << "left_data_mean " << left_data_mean << " left_data_sum " << left_data_sum << endl;
	cout << "left_data_variance_mean " << left_data_variance_mean << " left_data_variance_sum " << left_data_variance_sum << endl;

	double x = left_data_variance[0];
	double mean_y = left_data_mean;
	double variance_y = left_data_variance_mean;
	double p = 1 / sqrt(2 * M_PI * variance_y) 
		* exp( (- pow(x - mean_y, 2) ) / (2 * variance_y) );

	double posterior = left_chance * p;

	cout << "posterior " << posterior << endl;
	cout << endl;
}

string GNB::predict(vector<double>)
{
	/*
	Once trained, this method is called and expected to return
	a predicted behavior for the given observation.

	INPUTS

	observation - a 4 tuple with s, d, s_dot, d_dot.
	- Example: [3.5, 0.1, 8.5, -0.2]

	OUTPUT

	A label representing the best guess of the classifier. Can
	be one of "left", "keep" or "right".
	"""
	# TODO - complete this
	*/

	return this->possible_labels[1];

}