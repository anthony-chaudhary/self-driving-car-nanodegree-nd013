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

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Extended Kalman Filter equations
 

  VectorXd Hx = VectorXd(3);
  float ro = z(0);
  float phi = z(1);
  float px = ro * cos(phi);
  float py = ro * sin(phi);
  float v = z(2);
  float vx = v * cos(phi);
  float vy = v * sin(phi);
  float first = sqrt(pow(px, 2) + pow(py, 2));
  float second = atan(py/px);
  float third = (px*vx + py*vy) / first;
  Hx << first, second, third;
  VectorXd x_state = VectorXd(4);
  x_state << px, py, vx, vy;
  */

  Tools tools = Tools::Tools();
  MatrixXd Hj = tools.CalculateJacobian(.x_);
  VectorXd y = z - Hx;
}
