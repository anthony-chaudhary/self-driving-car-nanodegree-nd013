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
  std_a_ = 2;

  // Process noise standard deviation yaw acceleration in rad/s^2
  std_yawdd_ = .7;

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

  //set augmented dimension
  n_aug_ = 7;

  //set state dimension
  n_x_ = 5;

  //define spreading parameter
  lambda_ = 3 - n_x_;

  // initial state vector
  x_ = VectorXd(n_x_);

  // initial covariance matrix
  P_ = MatrixXd(n_x_, n_x_);

  weights_ = VectorXd(2 * n_aug_ + 1);
  int sigma_p_length  = (n_aug_ * 2) + 1;

  // set weights, mean
  double lambda_n_aug = lambda_ + n_aug_;
  double weight_i     = .5 / lambda_n_aug;

  weights_(0) = lambda_ / lambda_n_aug;
  // remainder of weights
  for (int i = 1;   i < sigma_p_length;   i++){
    weights_(i) = weight_i;
  }

  //create augmented mean vector
  x_aug = VectorXd(7);

  //create augmented state covariance
  P_aug = MatrixXd(7, 7);

  //create sigma point matrix
  Xsig_aug = MatrixXd(n_aug_,   2 * n_aug_ + 1);

  //create matrix with predicted sigma points as columns
  Xsig_pred_ = MatrixXd(n_x_,   2 * n_aug_ + 1);

  //set measurement dimension, radar can measure r, phi, and r_dot
  int n_z_ = 3;

  //create matrix for sigma points in measurement space
  Zsig = MatrixXd(n_z_, 2 * n_aug_ + 1);

  //mean predicted measurement
  z_pred = VectorXd(n_z_);
  
  //measurement covariance matrix S
  S = MatrixXd(n_z_, n_z_);

  // Create vector for incoming radar measurement
  z = VectorXd(n_z_);

  // Create matrix for cross correlation Tc
  Tc = MatrixXd(n_x_, n_z_);

  is_initialized_ = false;

  // Add measurement noise to covariance matrix
  R_laser = MatrixXd(2, 2);
  R_laser <<  std_laspx_,  0,
              0,        std_laspy_;

  H_laser_ = MatrixXd(2, 5);
  H_laser_ << 1, 0, 0, 0, 0,
              0, 1, 0, 0, 0;

  // Create vector for incoming measurement
  z_laser = VectorXd(4);

  previous_timestamp_ = 0;

}

UKF::~UKF() {}

void UKF::ProcessMeasurement(MeasurementPackage meas_package) {
  /*
   * @param {MeasurementPackage} meas_package The latest measurement data of
   * either radar or laser.
   */

  if (!is_initialized_) {

    P_ <<     1,   0,   0,   0,   0,
              0,   1,   0,   0,   0,
              0,   0,   1,   0,   0,
              0,   0,   0,   1,   0,
              0,   0,   0,   0,   1;

    if (meas_package.sensor_type_ == MeasurementPackage::RADAR) {
      // Convert radar from polar to cartesian coordinates and initialize state.
      /* Where 
       *  rho == distance from origin to point p 
       *  phi == angle between px and py
       *  rhodot == estimated distance from point p ?
       */
      double rho = meas_package.raw_measurements_[0];
      double phi = meas_package.raw_measurements_[1];

      double px = rho * cos(phi);
      double py = rho * sin(phi);

      if (px < 0.001) px = 0.001;
      if (py < 0.001) py = 0.001;

      x_ <<   px, 
              py,
              0, 
              0, 
              0;
    }

    else if (meas_package.sensor_type_ == MeasurementPackage::LASER) {
      // set state with initial location and zero velocity
      // where m[0] == point_x, and m[1] == point_y

      double px = meas_package.raw_measurements_[0];
      double py = meas_package.raw_measurements_[1];

      if (px < 0.001) px = 0.001;
      if (py < 0.001) py = 0.001;

      x_ <<   px, 
              py,
              0, 
              0, 
              0;
    }

    // done initializing, no need to predict or update
    is_initialized_ = true;
    previous_timestamp_ = meas_package.timestamp_;
    // cout << "is_initialized_ complete." << endl;

    return;
  }

  /*****************************************************************************
   *  Prediction
   ****************************************************************************/

  // 1) compute elapsed time where delta time (dt) == difference_time, expresssed in seconds
  
  double secondsDivisor = 1000000.0; // 1,000,000
  double delta_t = (meas_package.timestamp_ - previous_timestamp_) / secondsDivisor;

  previous_timestamp_ = meas_package.timestamp_;

  // cout << "delta_t: " << delta_t << endl;

  // If time difference is acceptable do prediction step. 
  if (delta_t > .001) {

    Prediction(delta_t);

  }

  /*****************************************************************************
   *  Update
   ****************************************************************************/

  if (meas_package.sensor_type_ == MeasurementPackage::RADAR) {

    UpdateRadar(meas_package);

  } else {  // Laser updates

    UpdateLidar(meas_package);
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
  // cout << "Starting Prediction()." << endl;

  /*
  MatrixXd Xsig = MatrixXd(n_x_, 2 * n_x_ + 1);  //create sigma point matrix
  MatrixXd A = P_.llt().matrixL();  // Calculate square root of P

  Xsig.col(0) << x_;  // Set first point equal to mean (where x == example state)

  // Set remaining points while i is less than n_x_ (state dimension)
  for (int i = 0;   i < n_x_;   i++)
  {
    Xsig.col(i + 1)         = x_ + sqrt(lambda_ + n_x_) * A.col(i);
    Xsig.col(i + 1 + n_x_)  = x_ - sqrt(lambda_ + n_x_) * A.col(i); 
  }

  cout << "Generated sigma points." << endl;
  */

  /*******************************************************************************
   * 1.1 Augment sigma points
   ******************************************************************************/
  n_aug_ = 7;

  //create augmented mean state
  x_aug.head(5) = x_;

  x_aug(5) = 0;
  x_aug(6) = 0;

  //create augmented covariance matrix
  P_aug.fill(0.0);
  P_aug.topLeftCorner(5, 5) = P_;
  P_aug(5, 5) = std_a_ * std_a_;
  P_aug(6, 6) = std_yawdd_ * std_yawdd_; 

  // cout << "Covariance matrix built." << endl;

  //create square root matrix
  MatrixXd L = P_aug.llt().matrixL();

  //create augmented sigma points
  Xsig_aug.col(0) = x_aug;

  for (int i = 0;   i < n_aug_;   i++)
  {
    Xsig_aug.col(i + 1)           = x_aug + sqrt(lambda_ + n_aug_) * L.col(i);
    Xsig_aug.col(i + 1 + n_aug_)  = x_aug - sqrt(lambda_ + n_aug_) * L.col(i); 
  }

  // cout << "Xsig_aug" << endl << Xsig_aug << endl;
  // cout << "Generated augmented sigma points." << endl;

  /*******************************************************************************
   * 2. Predict sigma points
   ******************************************************************************/

  int n_aug_limit = 2 * 7 + 1;
  // cout << "n_aug_limit" << endl << n_aug_limit << ", " << n_aug_ << endl;

  for (int i =0;  i < n_aug_limit;  i++)
  {
    // Declare vector points to increase clarity
    const double point_x          = Xsig_aug(0, i);
    const double point_y          = Xsig_aug(1, i);
    const double velocity         = Xsig_aug(2, i);
    const double yaw              = Xsig_aug(3, i);
    const double yaw_diff         = Xsig_aug(4, i);
    const double nu_a             = Xsig_aug(5, i);
    const double nu_yaw_diff_diff = Xsig_aug(6, i);

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

  // cout << "Sigma points prediction complete." << endl << Xsig_pred_ << endl;

  /*******************************************************************************
   * 3. Predict mean and covariance
   ******************************************************************************/

  int sigma_p_length  = (7 * 2) + 1;

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

  // cout << "P_" << endl << P_ << endl;
  // cout << "Prediction complete." << endl;

}


void UKF::UpdateLidar(MeasurementPackage meas_package) {
  /**
   * Updates the state and the state covariance matrix using a laser measurement.
   * @param {MeasurementPackage} meas_package
   * Use lidar data to update the belief about the object's
   */

  /**   TODO: Calculate the lidar NIS.
  */

  // cout << "Starting laser update." << endl;

  /*******************************************************************************
   * Laser Update
   ******************************************************************************/

  z_laser =  meas_package.raw_measurements_;

  // cout << "z_laser" << z_laser.size() << endl << z_laser << endl;

  VectorXd z_pred = H_laser_ * x_;
  VectorXd y_ = z_laser - z_pred;  // new filter for error calculation

  MatrixXd H_transpose = H_laser_.transpose();
  MatrixXd P_h_transpose = P_ * H_transpose;

  MatrixXd S_ = H_laser_ * P_h_transpose + R_laser;
  MatrixXd S_inverse = S_.inverse();

  MatrixXd K_ = P_h_transpose * S_inverse;

  // new estimate
  x_ = x_ + (K_ * y_);
  long x_size = x_.size();
  MatrixXd I_ = MatrixXd::Identity(x_size, x_size);
  P_ = (I_ - K_ * H_laser_) * P_;

  NIS_laser_ = y_.transpose() * S_inverse * y_;

  // cout << "Laser update complete." << endl;

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

  // cout << "Starting Radar update." << endl;

  int sigma_p_length = 2 * 7 + 1;
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
  MatrixXd R_(3, 3);
  R_ <<  std_radr_ * std_radr_,  0,                          0,
        0,                      std_radphi_ * std_radphi_,  0,
        0,                      0,                          std_radrd_ * std_radrd_;

  S += R_;

  // cout << "Radar measurement complete." << endl;


  /*******************************************************************************
   * 2. Radar Update
   ******************************************************************************/

  VectorXd z = meas_package.raw_measurements_;

  // Calculate cross correlation matrix
  int simga_p_length = 7 * 2 + 1;
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

    // cout << "Radar normalization complete." << endl;

    // Update TC
    Tc += weights_(i) * x_diff * z_diff_i.transpose();
  }

  // Calculate Kalman gain K;
  MatrixXd K = Tc * S.inverse();

  // cout << "Radar Kalman gain complete." << endl;

  // Precompute z_diff to do normalization before state update
  VectorXd z_diff = z - z_pred;

  // Angle normalization
  while (z_diff(1) >  M_PI)  z_diff(1) -= 2. * M_PI;
  while (z_diff(1) <  -M_PI) z_diff(1) += 2. * M_PI;

  /* 
  cout << "Starting covariance update." << endl;
  cout << "x_ size:" << x_.size() << endl << x_<< endl;
  cout << "K size:" << K.size() << endl << K << endl;
  cout << "z_diff size:" << z_diff.size() << endl << z_diff << endl;
  cout << "S size:" << S.size() << endl << S << endl;

  cout << "Xsig_pred_ size:" << Xsig_pred_.size() << endl << Xsig_pred_ << endl;
  cout << "P_" << endl << P_ << endl;
  cout << "K" << endl << K << endl;
  cout << "z" << endl << z << endl;
  cout << "z_diff" << endl << z_diff << endl;
  cout << "S" << S << endl;

  */

  // Update state mean and covariance matrix
  x_ += K * z_diff;
  P_ += -(K * S * K.transpose());

  // cout << "State and covariance matrix updated." << endl;

  NIS_radar_ = z_diff.transpose() * S.inverse() * z_diff;

  // cout << "Radar complete." << endl;

}
