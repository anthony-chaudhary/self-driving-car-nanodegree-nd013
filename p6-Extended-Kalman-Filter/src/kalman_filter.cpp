#include "kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;  // object state
  P_ = P_in;  // object covariance matrix
  F_ = F_in;  // state transistion matrix
  H_ = H_in;  // measurement matrix
  R_ = R_in;  // measurement covariance matrix
  Q_ = Q_in;  // process covariance matrix
}

void KalmanFilter::Predict() {
  /*
   * Predict the state
   */

  x_ = (F_ * x_);

  MatrixXd F_transpose = F_.transpose();
  P_ = F_ * P_ * F_transpose + Q_;

}

void KalmanFilter::Update(const VectorXd &z) {
  /*
   * Update the state
   */

  VectorXd z_pred = H_ * x_;
  VectorXd y_ = z - z_pred;  // new filter for error calculation

  MatrixXd H_transpose = H_.transpose();
  MatrixXd P_h_transpose = P_ * H_transpose;

  MatrixXd S_ = H_ * P_h_transpose + R_;
  MatrixXd S_inverse = S_.inverse();

  MatrixXd K_ = P_h_transpose * S_inverse;

  // new estimate
  x_ = x_ + (K_ * y_);
  long x_size = x_.size();
  MatrixXd I_ = MatrixXd::Identity(x_size, x_size);
  P_ = (I_ - K_ * H_) * P_;

}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /*
   * update the state by using Extended Kalman Filter equations
   */

  // convert to polar
  VectorXd new_polar(3);
  new_polar[0] = sqrt(x_[0] * x_[0] + x_[1] * x_[1]);
  new_polar[1] = atan2(x_[1], x_[0]);
  double d = new_polar[0];
  if (d < 1e-6) d = 1e-6;
  new_polar[2] = (x_[0] * x_[2] + x_[1] * x_[3]) / d;

  VectorXd z_pred = new_polar;
  VectorXd y_ = z - z_pred;

  // This part same as normal update
  MatrixXd H_transpose = H_.transpose();
  MatrixXd S_ = H_ * P_ * H_transpose + R_;

  MatrixXd S_inverse = S_.inverse();

  MatrixXd P_h_transpose = P_ * H_transpose;
  MatrixXd K_ = P_h_transpose * S_inverse;

  // new estimate
  x_ = x_ + (K_ * y_);
  long x_size = x_.size();
  MatrixXd I_ = MatrixXd::Identity(x_size, x_size);
  P_ = (I_ - K_ * H_) * P_;
}
