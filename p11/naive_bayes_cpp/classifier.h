#ifndef CLASSIFIER_H
#define CLASSIFIER_H
#include <iostream>
#include <sstream>
#include <fstream>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

using namespace std;

class GNB {
public:

	vector<string> possible_labels = { "left","keep","right" };


	/**
	* Constructor
	*/
	GNB();

	/**
	* Destructor
	*/
	virtual ~GNB();

	void train(vector<vector<double> > data, vector<string>  labels);

	string predict(vector<double>);

};

#endif



