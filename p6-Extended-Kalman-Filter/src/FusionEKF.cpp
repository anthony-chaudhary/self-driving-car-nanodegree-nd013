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

  previous_timestamp_ = 0;

  // initializing matrices
  R_laser_ = MatrixXd(2, 2);
  R_radar_ = MatrixXd(3, 3);
  H_laser_ = MatrixXd(2, 4);
  Hj_ = MatrixXd(3, 4);

  //measurement covariance matrix - laser
  R_laser_ << 0.0225, 0,
        0, 0.0225;

  //measurement covariance matrix - radar
  R_radar_ << 0.09, 0, 0,
        0, 0.0009, 0,
        0, 0, 0.09;
}

/**
* Destructor.
*/
FusionEKF::~FusionEKF() {}

void FusionEKF::ProcessMeasurement(const MeasurementPackage &measurement_pack) {

  /*****************************************************************************
   *  Initialization
   ****************************************************************************/
  // acceleration noise components for Q matrix.
  int noise_ax = 9;
  int noise_ay = 9;

  if (!is_initialized_) {

    //measurement covariance
    ekf_.R_ = MatrixXd(2, 2);
    ekf_.R_ << 0.0225, 0,
          0, 0.0225;

    // first measurement
    cout << "EKF: " << endl;
    ekf_.x_ = VectorXd(4);
    ekf_.x_ << 1, 1, 1, 1;

    if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
      // Convert radar from polar to cartesian coordinates and initialize state.
      /* Where 
       *  rho == distance from origin to point p 
       *  phi == angle between px and py
       *  rhodot == estiamted distance from point p ?
       */
      auto rho = measurement_pack.raw_measurements_(0);
      auto phi = measurement_pack.raw_measurements_(1);
      auto rhodot = measurement_pack.raw_measurements_(2);

      auto px = rho * cos(phi);
      auto py = rho * sin(phi);

      auto vx = rhodot * cos(phi);
      auto vy = rhodot * sin(phi);

      ekf_.x_ << px, py, vx, vy;
    }

    else if (measurement_pack.sensor_type_ == MeasurementPackage::LASER) {
      // set state with initial llocation and zero velocity
      ekf_.x_ << measurement_pack.raw_measurements_[0], measurement_pack.raw_measurements_[1], 0, 0;
    }

    // functions valid for both laser and radar

    previous_timestamp_ = measurement_pack.timestamp_;

    // done initializing, no need to predict or update
    is_initialized_ = true;
    return;
  }

  /*****************************************************************************
   *  Prediction
   ****************************************************************************/

  ekf_.Predict();

  /*****************************************************************************
   *  Update
   ****************************************************************************/

  // compute elapsed time where dt == difference_time, expresssed in seconds
  float secondsDivisor = 1000000.0; // 1,000,000
  float dt = (measurement_pack.timestamp_ - previous_timestamp_) / secondsDivisor;

  // Pre computer time difference power terms 
  // where dt_x == "_x" == power term ie dt_3 == dt to the third power
  float dt_2 = dt * dt;
  float dt_3 = dt_2 * dt;
  float dt_4 = dt_3 * dt;
  // end precomputation of dt terms

  // modfy F matrix to integrate time
  ekf_.F_(0, 2) = dt;
  ekf_.F_(1, 3) = dt;


  if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
    // Radar updates


  } else {  // Laser updates

    // set covariance matrix Q
    ekf_.Q_ = MatrixXd(4, 4);
    ekf_.Q_ <<  dt_4/4*noise_ax, 0, dt_3/2*noise_ax, 0,
                 0, dt_4/4*noise_ay, 0, dt_3/2*noise_ay,
                 dt_3/2*noise_ax, 0, dt_2*noise_ax, 0,
                 0, dt_3/2*noise_ay, 0, dt_2*noise_ay;

    // Update (prediction handled above)
    ekf_.Update(measurement_pack.raw_measurements_);

  }

  // print the output
  cout << "x_ = " << ekf_.x_ << endl;
  cout << "P_ = " << ekf_.P_ << endl;
}
