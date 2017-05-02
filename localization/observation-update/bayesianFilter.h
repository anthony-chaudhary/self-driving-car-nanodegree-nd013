//============================================================================
// Name        : bayesianFilter.h
// Version     : 1.0.0
// Copyright   : MBRDNA, Udacity
//============================================================================

#ifndef BAYESIANFILTER_H_
#define BAYESIANFILTER_H_

#include <vector>
#include <string>
#include <fstream>

#include "measurement_package.h"
#include "map.h"
#include "help_functions.h"

class bayesianFilter {
public:
	//constructor:
	bayesianFilter();
	//deconstructor:
	virtual ~bayesianFilter();


	//main public member function, which estimate the beliefs:
	void process_measurement(const MeasurementPackage &measurements,
			                 const map &map_1d,
			                 help_functions &helpers);

	//member public: belief of state x:
	std::vector<float> bel_x ;

private:

/////private members:

	//flag, if filter is initialized:
	bool is_initialized_;

	//precision of control information:
	float control_std ;

	//initial belief of state x:
	std::vector<float> bel_x_init ;

};

#endif /* BAYESIANFILTER_H_ */