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
	/**
	* 
	*/

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


MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {
	/**
	* 
	*/

	MatrixXd Hj(3, 4);

	// state parameters

	double px = x_state(0);
	double py = x_state(1);

	double vx = x_state(2);
	double vy = x_state(3);

	// for efficiency terms pre-computed here
	double c1 = (px * px) + (py * py);

	// division by zero check
	if(fabs(c1) < 0.0001) {
		cout << "CalculateJacobian () - Warning - division by Zero" << endl;
		c1 = 1;
	}

	double c2 = sqrt(c1);
	double c3 = (c1 * c2);
	// end pre-compute


	Hj << (px / c2), (py / c2), 0 , 0,
		  -(py / c1), (px / c1), 0 , 0,
		  py * (vx * py - vy * px) / c3, px * (px * vy - py * vx) / c3, px/c2, py/c2;

		  return Hj;

}
