#include "FusionEKF.h"
#include "tools.h"
#include "Eigen/Dense"
#include <iostream>

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::vector;

/*
 * Constructor.
 */
FusionEKF::FusionEKF() {

  is_initialized_ = false;
  long long previous_timestamp_ = 0;

  // initializing matrices
  R_laser_ = MatrixXd(2, 2);
  R_radar_ = MatrixXd(3, 3);

  H_laser_ = MatrixXd(2, 4);
  H_laser_ << 1, 0, 0, 0,
              0, 1, 0, 0;

  // Hj_ = MatrixXd(3, 4);

  //measurement covariance matrix - laser
  R_laser_ << 0.0225, 0,
              0,      0.0225;

  //measurement covariance matrix - radar
  R_radar_ << 0.09, 0,      0,
              0,    0.0009, 0,
              0,    0,      0.09;

  ekf_.P_ = MatrixXd(4, 4);
  ekf_.P_ <<  .5,    0,    0,      0,
              0,    .5,    0,      0,
              0,    0,    1000,   0,
              0,    0,    0,    1000;

  // state transistion matrix
  // will change with time
  ekf_.F_ = MatrixXd(4, 4);
  ekf_.F_ << 1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1;

  ekf_.Q_ = MatrixXd(4, 4);
  // appears to need this to run otherwise get nan(ind) error
  ekf_.Q_ << 0, 0, 0, 0,
             0, 0, 0, 0,
             0, 0, 0, 0,
             0, 0, 0, 0;

}

/**
* Destructor.
*/
FusionEKF::~FusionEKF() {}

void FusionEKF::ProcessMeasurement(const MeasurementPackage &measurement_pack) {

  /*****************************************************************************
   *  Initialization
   ****************************************************************************/
  if (!is_initialized_) {

    // first measurement
    // cout << "EKF: " << endl;
    ekf_.x_ = VectorXd(4);
    ekf_.x_ << 1, 1, 1, 1;

    if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
      // Convert radar from polar to cartesian coordinates and initialize state.
      /* Where 
       *  rho == distance from origin to point p 
       *  phi == angle between px and py
       *  rhodot == estimated distance from point p ?
       */
      double rho = measurement_pack.raw_measurements_[0];
      double phi = measurement_pack.raw_measurements_[1];
      double rhodot = measurement_pack.raw_measurements_[2];

      double px = rho * cos(phi);
      double py = rho * sin(phi);

      double vx = rhodot * cos(phi);
      double vy = rhodot * sin(phi);

      ekf_.x_ << px, py, vx, vy;
    }

    else if (measurement_pack.sensor_type_ == MeasurementPackage::LASER) {
      // set state with initial llocation and zero velocity
      ekf_.x_ << measurement_pack.raw_measurements_[0], measurement_pack.raw_measurements_[1], 0, 0;
    }

    // functions valid for both laser and radar
    long long previous_timestamp_ = measurement_pack.timestamp_;
    // done initializing, no need to predict or update
    is_initialized_ = true;

    return;
  }

  /*****************************************************************************
   *  Prediction
   ****************************************************************************/

  // 1) compute elapsed time where delta time (dt) == difference_time, expresssed in seconds
  double secondsDivisor = 1000000.0; // 1,000,000
  double dt = (measurement_pack.timestamp_ - previous_timestamp_) / secondsDivisor;

  // Pre computer time difference power terms 
  // where dt_x == "_x" == power term ie dt_3 == dt to the third power
  double dt_2 = dt * dt;
  double dt_3 = dt_2 * dt;
  double dt_4 = dt_3 * dt;
  // end precomputation of dt terms

  // 2) use Delta t to compute new EKF F, Q

  // modfy F matrix to integrate time
  ekf_.F_(0, 2) = dt;
  ekf_.F_(1, 3) = dt;

  // acceleration noise components for Q matrix.
  const int noise_ax = 9;
  const int noise_ay = 9;

    // set covariance matrix Q
  ekf_.Q_ = MatrixXd(4, 4);
  ekf_.Q_ <<  dt_4/4*noise_ax, 0,               dt_3/2*noise_ax, 0,
              0,               dt_4/4*noise_ay, 0,               dt_3/2*noise_ay,
              dt_3/2*noise_ax, 0,               dt_2*noise_ax,   0,
              0,               dt_3/2*noise_ay, 0,               dt_2*noise_ay;


  // If time difference is acceptable do prediction step. 
  if (dt > .001) {
    ekf_.Predict();
  }

  /*****************************************************************************
   *  Update
   ****************************************************************************/

  // Setup matrices
  if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {

    // Lineraize measurement function
    ekf_.H_ = tools.CalculateJacobian(ekf_.x_);

    // Radar updates
    ekf_.R_ = R_radar_;

    // State update with new measurements
    ekf_.UpdateEKF(measurement_pack.raw_measurements_);

  } else {  // Laser updates
    ekf_.R_ = R_laser_;
    ekf_.H_ = H_laser_;
    // State update with new measurements
    ekf_.Update(measurement_pack.raw_measurements_);
  }

  // print the output
  // cout << "x_ = " << ekf_.x_ << endl;
  // cout << "P_ = " << ekf_.P_ << endl;
}
