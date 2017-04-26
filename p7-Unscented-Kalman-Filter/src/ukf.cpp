#include "ukf.h"
#include "tools.h"
#include "Eigen/Dense"
#include <iostream>

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::vector;

/**
 * Initializes Unscented Kalman filter
 */
UKF::UKF() {
  // if this is false, laser measurements will be ignored (except during init)
  use_laser_ = true;

  // if this is false, radar measurements will be ignored (except during init)
  use_radar_ = true;

  // Process noise standard deviation longitudinal acceleration in m/s^2
  std_a_ = .2;

  // Process noise standard deviation yaw acceleration in rad/s^2
  std_yawdd_ = .2;

  // Laser measurement noise standard deviation position1 in m
  std_laspx_ = 0.15;

  // Laser measurement noise standard deviation position2 in m
  std_laspy_ = 0.15;

  // Radar measurement noise standard deviation radius in m
  std_radr_ = 0.3;

  // Radar measurement noise standard deviation angle in rad
  std_radphi_ = 0.03;

  // Radar measurement noise standard deviation radius change in m/s
  std_radrd_ = 0.3;

  /**
  TODO:

  Complete the initialization. See ukf.h for other member properties.

  Hint: one or more values initialized above might be wildly off...
  */

  //set augmented dimension
  int n_aug_ = 7;

  //set state dimension
  int n_x_ = 5;

  //define spreading parameter
  double lambda_ = 3 - n_x_;

  // initial state vector
  x_ = VectorXd(n_x_);
  x_ <<   5.7441,
         1.3800,
         2.2049,
         0.5015,
         0.3528;

  // initial covariance matrix
  P_ = MatrixXd(n_x_, n_x_);
  P_ <<     0.0043,   -0.0013,    0.0030,   -0.0022,   -0.0020,
          -0.0013,    0.0077,    0.0011,    0.0071,    0.0060,
           0.0030,    0.0011,    0.0054,    0.0007,    0.0008,
          -0.0022,    0.0071,    0.0007,    0.0098,    0.0100,
          -0.0020,    0.0060,    0.0008,    0.0100,    0.0123;

  //create matrix with predicted sigma points as columns
  MatrixXd Xsig_pred_ = MatrixXd(n_x_, 2 * n_aug_ + 1);

  weights_ = VectorXd(2 * n_aug_ + 1);

}

UKF::~UKF() {}

void UKF::ProcessMeasurement(MeasurementPackage meas_package) {
  /*
   * @param {MeasurementPackage} meas_package The latest measurement data of
   * either radar or laser.
   */

  if (!is_initialized_) {

    // first measurement
    x_ = VectorXd(4);
    x_ << 1, 1, 1, 1;

    if (meas_package.sensor_type_ == MeasurementPackage::RADAR) {
      // Convert radar from polar to cartesian coordinates and initialize state.
      /* Where 
       *  rho == distance from origin to point p 
       *  phi == angle between px and py
       *  rhodot == estimated distance from point p ?
       */
      double rho = meas_package.raw_measurements_[0];
      double phi = meas_package.raw_measurements_[1];
      double rhodot = meas_package.raw_measurements_[2];

      double px = rho * cos(phi);
      double py = rho * sin(phi);

      double vx = rhodot * cos(phi);
      double vy = rhodot * sin(phi);

      x_ << px, py, vx, vy;
    }

    else if (meas_package.sensor_type_ == MeasurementPackage::LASER) {
      // set state with initial location and zero velocity
      // where m[0] == point_x, and m[1] == point_y
      x_ <<  meas_package.raw_measurements_[0], 
             meas_package.raw_measurements_[1], 
             0,
             0;
    }

    // functions valid for both laser and radar
    long long previous_timestamp_ = meas_package.timestamp_;

    // done initializing, no need to predict or update
    is_initialized_ = true;

    return;
  }
}


void UKF::Prediction(double delta_t) {
  /*
   * Predicts sigma points, the state, and the state covariance matrix.
   * @param {double} delta_t the change in time (in seconds) between the last
   * measurement and this one.
   * Process
   * 1.   Generate sigma points
   * 1.1  Augment sigma points
   * 2.   Predict sigma points
   * 3.   Predict mean and covariance
   */


  /*******************************************************************************
   * 1. Generate sigma points
   ******************************************************************************/

  MatrixXd Xsig = MatrixXd(n_x_, 2 * n_x_ + 1);  //create sigma point matrix
  MatrixXd A = P_.llt().matrixL();  // Calculate square root of P

  Xsig.col(0) << x_;  // Set first point equal to mean (where x == example state)

  // Set remaining points while i is less than n_x_ (state dimension)
  for (int i = 0;   i < n_x_;   i++)
  {
    Xsig.col(i + 1)         = x_ + sqrt(lambda_ + n_x_) * A.col(i);
    Xsig.col(i + 1 + n_x_)  = x_ - sqrt(lambda_ + n_x_) * A.col(i); 
  }


  /*******************************************************************************
   * 1.1 Augment sigma points
   ******************************************************************************/

  //create augmented mean vector
  VectorXd x_aug = VectorXd(7);

  //create augmented state covariance
  MatrixXd P_aug = MatrixXd(7, 7);

  //create sigma point matrix
  MatrixXd Xsig_aug = MatrixXd(n_aug_, 2 * n_aug_ + 1);

  //create augmented mean state
  x_aug.head(5) = x_;
  x_aug(5) = 0;
  x_aug(6) = 0;

  //create augmented covariance matrix
  P_aug.fill(0.0);
  P_aug.topLeftCorner(5, 5) = P_;
  P_aug(5, 5) = std_a_ * std_a_;
  P_aug(6, 6) = std_yawdd_ * std_yawdd_;
  //P_aug.bottomRightCorner( (P))  

  //create square root matrix
  MatrixXd L = P_aug.llt().matrixL();

  //create augmented sigma points
  Xsig_aug.col(0) = x_aug;

  for (int i = 0;   i < n_aug_;   i++)
  {
    Xsig_aug.col(i + 1)          = x_aug + sqrt(lambda_ + n_aug_) * L.col(i);
    Xsig_aug.col(i + 1 + n_aug_)  = x_aug - sqrt(lambda_ + n_aug_) * L.col(i); 
  }


  /*******************************************************************************
   * 2. Predict sigma points
   ******************************************************************************/

  //predict sigma points
  int n_aug_limit = 2 * n_aug_ + 1;
  for (int i =0;  i < n_aug_limit;  i++)
  {
    // Declare vector points to increase clarity
    double point_x          = Xsig_aug(0, i);
    double point_y          = Xsig_aug(1, i);
    double velocity         = Xsig_aug(2, i);
    double yaw              = Xsig_aug(3, i);
    double yaw_diff         = Xsig_aug(4, i);
    double nu_a             = Xsig_aug(5, i);
    double nu_yaw_diff_diff = Xsig_aug(6, i);

    // Declare predicted state values
    double point_x_predicted, point_y_predicted, velocity_predicted, yaw_predicted, yaw_diff_predicted;
    // std::cout << "point_x_predicted = " << std::endl << point_x_predicted << std::endl;

    // pre-compute
    double yaw_diff_product_delta_t = yaw_diff * delta_t;
    double cos_yaw = cos(yaw);
    double sin_yaw = sin(yaw);

    //avoid division by zero
    if (fabs(yaw_diff) > 0.001) {
      point_x_predicted = point_x + velocity / yaw_diff * ( sin(yaw + yaw_diff_product_delta_t) - sin_yaw );
      point_y_predicted = point_y + velocity / yaw_diff * ( cos_yaw - cos(yaw + yaw_diff_product_delta_t) );
    }
    else {
      point_x_predicted = point_x + velocity * delta_t * cos_yaw;
      point_y_predicted = point_y + velocity * delta_t * sin_yaw;
    }

    // std::cout << "point_x_predicted = " << std::endl << point_x_predicted << std::endl;

    // pre-compute
    velocity_predicted      = velocity;
    yaw_predicted           = yaw + yaw_diff_product_delta_t;
    yaw_diff_predicted      = yaw_diff;

    double delta_t_squared  = delta_t * delta_t;
    double half_nu_a        = .5 * nu_a;

    // add noise
    point_x_predicted   += half_nu_a * delta_t_squared * cos_yaw;
    point_y_predicted   += half_nu_a * delta_t_squared * sin_yaw;
    velocity_predicted  += nu_a * delta_t;
    yaw_predicted       += .5 * nu_yaw_diff_diff * delta_t_squared;
    yaw_diff_predicted  += nu_yaw_diff_diff * delta_t;

    //write predicted sigma points into right column
    Xsig_pred_(0, i) = point_x_predicted;
    Xsig_pred_(1, i) = point_y_predicted;
    Xsig_pred_(2, i) = velocity_predicted;
    Xsig_pred_(3, i) = yaw_predicted;
    Xsig_pred_(4, i) = yaw_diff_predicted;

  }

  /*******************************************************************************
   * 3. Predict mean and covariance
   ******************************************************************************/

  // pre-compute
  double lambda_n_aug = lambda_ + n_aug_;
  double weight_i     = .5 / lambda_n_aug;
  int sigma_p_length  = (n_aug_ * 2) + 1;

  // set weights
  // mean
  weights_(0) = lambda_ / lambda_n_aug;
  // remainder of weights
  for (int i = 1;   i < sigma_p_length;   i++){
    weights_(i) = weight_i;
  }

  //predict state mean
  x_.fill(0.0);
  for (int i = 0;   i < sigma_p_length;   i++){
    x_ += weights_(i) * Xsig_pred_.col(i);
  }

  //predict state covariance matrix
  P_.fill(0.0);
  for (int i = 0;   i < sigma_p_length;   i++){
    
    // state difference
    VectorXd x_diff = Xsig_pred_.col(i) - x_;

    // normalize angles
    while (x_diff(3) >  M_PI) x_diff(3) -= 2. * M_PI;
    while (x_diff(3) < -M_PI) x_diff(3) += 2. * M_PI;

    P_ += weights_(i) * x_diff * x_diff.transpose();
  }
}


/**
 * Updates the state and the state covariance matrix using a laser measurement.
 * @param {MeasurementPackage} meas_package
 */
void UKF::UpdateLidar(MeasurementPackage meas_package) {
  /**
  TODO:

  Complete this function! Use lidar data to update the belief about the object's
  position. Modify the state vector, x_, and covariance, P_.

  You'll also need to calculate the lidar NIS.
  */

}


void UKF::UpdateRadar(MeasurementPackage meas_package) {
  /**
   * Updates the state and the state covariance matrix using a radar measurement.
   * @param {MeasurementPackage} meas_package
   */

  /**
  TODO: Calculate the radar NIS.
  */

  /*******************************************************************************
   * 1. Radar measurement --> Update belief about the object's position.
   ******************************************************************************/

  //set measurement dimension, radar can measure r, phi, and r_dot
  int n_z_ = 3;

  //create matrix for sigma points in measurement space
  MatrixXd Zsig = MatrixXd(n_z_, 2 * n_aug_ + 1);

  //mean predicted measurement
  VectorXd z_pred = VectorXd(n_z_);
  
  //measurement covariance matrix S
  MatrixXd S = MatrixXd(n_z_, n_z_);

  int sigma_p_length = 2 * n_aug_ + 1;
  z_pred.fill(0.0);
  S.fill(0.0);

  for (int i = 0;   i < sigma_p_length;   i++){
    // Transform sigma points into measurement space
    VectorXd SigmaPolar(3);
    double x_point  = Xsig_pred_(0, i);
    double y_point  = Xsig_pred_(1, i);
    double velocity = Xsig_pred_(2, i);
    double psi      = Xsig_pred_(3, i);

    double rho = sqrt(x_point * x_point + y_point * y_point);
    if (rho < 1e-6) rho = 1e-6;  // Divide by 0 safe for rhodot

    double theta = atan2(y_point, x_point);

    double rhodot = (x_point * cos(psi) * velocity + y_point * sin(psi) * velocity) / rho;

    SigmaPolar << rho, theta, rhodot;

    //calculate mean predicted measurement
    z_pred += weights_(i) * SigmaPolar;

    // Add SigmaPolar points into Zsig for use in future calculations
    Zsig.col(i) << SigmaPolar;
  }

  for (int i = 0;   i < sigma_p_length;   i++){
    //calculate measurement covariance matrix S
    // resdiual
    VectorXd z_diff = Zsig.col(i) - z_pred;

    // angle normalization
    while (z_diff(1) >  M_PI)  z_diff(1) -= 2. * M_PI;
    while (z_diff(1) <  -M_PI) z_diff(1) += 2. * M_PI;

    S += weights_(i) * z_diff * z_diff.transpose();
  }

  // Add measurement noise to covariance matrix
  MatrixXd R_(n_z_, n_z_);
  R_ <<  std_radr_ * std_radr_,  0,                          0,
        0,                      std_radphi_ * std_radphi_,  0,
        0,                      0,                          std_radrd_ * std_radrd_;

  S += R_;


  /*******************************************************************************
   * 2. Radar Update
   ******************************************************************************/

  // Create vector for incoming radar measurement
  VectorXd z = VectorXd(n_z_);
  z <<
      meas_package.raw_measurements_[0],   //rho in m
      meas_package.raw_measurements_[1],   //phi in rad
      meas_package.raw_measurements_[2];   //rho_dot in m/s

  // Create matrix for cross correlation Tc
  MatrixXd Tc = MatrixXd(n_z_, n_z_);

  // Calculate cross correlation matrix
  int simga_p_length = n_aug_ * 2 + 1;
  Tc.fill(0.0);

  for (int i = 0;   i < simga_p_length;   i++){

    // Pre-compute z_diff and x_diff for normalization
    VectorXd x_diff   = Xsig_pred_.col(i) - x_;
    VectorXd z_diff_i = Zsig.col(i) - z_pred;

    // Normalization
    while (z_diff_i(1) >  M_PI)  z_diff_i(1) -= 2. * M_PI;
    while (z_diff_i(1) <  -M_PI) z_diff_i(1) += 2. * M_PI;

    while (x_diff(3) >  M_PI)  x_diff(3) -= 2. * M_PI;
    while (x_diff(3) <  -M_PI) x_diff(3) += 2. * M_PI;

    // Update TC
    Tc += weights_(i) * x_diff * z_diff_i.transpose();
  }

  // Calculate Kalman gain K;
  MatrixXd K = Tc * S.inverse();

  // Precompute z_diff to do normalization before state update
  VectorXd z_diff = z - z_pred;

  // Angle normalization
  while (z_diff(1) >  M_PI)  z_diff(1) -= 2. * M_PI;
  while (z_diff(1) <  -M_PI) z_diff(1) += 2. * M_PI;

  // Update state mean and covariance matrix
  x_ += K * z_diff;
  P_ += -(K * S * K.transpose());

}
