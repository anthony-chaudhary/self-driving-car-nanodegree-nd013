#include <iostream>
#include "tools.h"

using namespace std;
using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {

	VectorXd rmse(4);
	rmse << 0,0,0,0;

	// check the validity of the following inputs:
	//  * the estimation vector size should not be zero
	//  * the estimation vector size should equal ground truth vector size
	if(estimations.size() != ground_truth.size()
		|| estimations.size() == 0) {
		
		cout << "Invalid estimations or ground truth data" << endl;

		return rmse;
	}

	auto estimations_size = estimations.size();

	// accumlate squared residuals
	for(unsigned int i = 0;  i < estimations_size;  ++i){

		VectorXd residual = estimations[i] - ground_truth[i];

		// coefficient-wise multiplication
		auto residual_array = residual.array();
		residual = residual_array * residual_array;
		rmse += residual;
	}

	// calculate mean
	rmse = rmse / estimations_size;

	// squared root
	rmse = rmse.array().sqrt();

	// returns result
	return rmse;

}
