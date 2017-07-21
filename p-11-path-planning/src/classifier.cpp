#include <iostream>
#include <sstream>
#include <fstream>


#include <math.h>
#include <vector>
#include "classifier.h"
#include <algorithm>

/**
* Initializes GNB
*/
GNB::GNB() {}
GNB::~GNB() {}

vector<vector<double> > GNB::load_state(string file_name)
{
	ifstream in_state_(file_name.c_str(), ifstream::in);
	vector< vector<double >> state_out;
	string start;

	while (getline(in_state_, start))
	{

		vector<double> x_coord;

		istringstream ss(start);
		double a;
		ss >> a;
		x_coord.push_back(a);

		string value;

		while (getline(ss, value, ',')) {
			double b;
			ss >> b;
			x_coord.push_back(b);
		}

		state_out.push_back(x_coord);
	}
	return state_out;
}

vector<string> GNB::load_label(string file_name)
{
	ifstream in_label_(file_name.c_str(), ifstream::in);
	vector< string > label_out;
	string line;
	while (getline(in_label_, line))
	{
		istringstream iss(line);
		string label;
		iss >> label;

		label_out.push_back(label);
	}
	return label_out;

}
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

	double len_labels = labels.size();
	cout << "\nLen_labels " << len_labels << endl;

	// chance of being left
	vector<double> data_counts = { 0, 0 ,0 };
	vector<double> data_sums = { 0, 0, 0 };
	vector <vector<double> > data_variance;
	data_variance.resize(3);

	// refactored for all labels 
	// TODO do more than first data element
	for (size_t i = 0; i < len_labels; ++i) {
		int index;
		if (labels[i] == "left") {
			index = 0;
		}
		if (labels[i] == "keep") {
			index = 1;
		}
		if (labels[i] == "right") {
			index = 2;
		}
		data_counts[index] += 1;
		data_sums[index] += data[i][3];
		data_variance[index].push_back(data[i][3]);

	}

	cout << "Updating variables\n" << endl;
	// update variables
	results_.resize(3);
	for (size_t i = 0; i < data_counts.size(); ++i) {

		cout << possible_labels[i] << endl;
		double chance = data_counts[i] / len_labels;
		double data_mean = data_sums[i] / data_counts[i];

		double data_variance_sum = 0;
		for (size_t j = 0; j < data_variance[i].size(); ++j) {
			double difference = data_mean - data_variance[i][j];
			double square = pow(difference, 2);
			data_variance_sum += square;
		}

		double data_variance_mean = data_variance_sum / data_counts[i];

		results_[i].push_back(chance);
		results_[i].push_back(data_mean);
		results_[i].push_back(data_variance_mean);

		cout << "Chance " << chance << " count " << data_counts[i] << endl;
		cout << "data_mean " << data_mean << " data_sum " << data_sums[i] << endl;
		cout << "data_variance_mean " << data_variance_mean <<
			" data_variance_sum " << data_variance_sum << "\n" << endl;

	}

}

string GNB::predict(vector<double> observation)
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
	*/

	double x = observation[3];

	double mean_y, variance_y, p, posterior;
	vector<double> posteriors;

	for (size_t i = 0; i < 3; ++i) {

		mean_y = results_[i][1];
		variance_y = results_[i][2];

		p = (1 / sqrt(2 * M_PI * variance_y))
			* exp((-pow(x - mean_y, 2)) / (2 * variance_y));
		posterior = results_[i][0] * p;

		//cout << posterior << endl;

		posteriors.push_back(posterior);
	}

	int winner;
	// TODO replace this with max_element()
	if (posteriors[0] > posteriors[1] && posteriors[0] > posteriors[2]) {
		winner = 0;
	}
	if (posteriors[1] > posteriors[2] && posteriors[1] > posteriors[2]) {
		winner = 1;
	}
	if (posteriors[2] > posteriors[1] && posteriors[2] > posteriors[0]) {
		winner = 2;
	}

	return this->possible_labels[winner];

}