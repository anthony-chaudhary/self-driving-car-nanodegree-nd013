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
	
	GNB();
	virtual ~GNB();

	vector<string> possible_labels = { "left","keep","right" };
	vector <vector <double> >results_;

	vector<vector<double> > load_state(string file_name);
	vector<string> load_label(string file_name);

	void train(vector<vector<double> > data, vector<string>  labels);
	string predict(double obseravation);

};

#endif



