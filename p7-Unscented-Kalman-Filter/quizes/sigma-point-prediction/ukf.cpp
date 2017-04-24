#include <iostream>
#include "ukf.h"

UKF::UKF() {
  //TODO Auto-generated constructor stub
  Init();
}

UKF::~UKF() {
  //TODO Auto-generated destructor stub
}

void UKF::Init() {

}


/*******************************************************************************
* Programming assignment functions: 
*******************************************************************************/

void UKF::SigmaPointPrediction(MatrixXd* Xsig_out) {

  //set state dimension
  int n_x = 5;

  //set augmented dimension
  int n_aug = 7;

  //create example sigma point matrix
  MatrixXd Xsig_aug = MatrixXd(n_aug, 2 * n_aug + 1);
     Xsig_aug <<
    5.7441,  5.85768,   5.7441,   5.7441,   5.7441,   5.7441,   5.7441,   5.7441,   5.63052,   5.7441,   5.7441,   5.7441,   5.7441,   5.7441,   5.7441,
      1.38,  1.34566,  1.52806,     1.38,     1.38,     1.38,     1.38,     1.38,   1.41434,  1.23194,     1.38,     1.38,     1.38,     1.38,     1.38,
    2.2049,  2.28414,  2.24557,  2.29582,   2.2049,   2.2049,   2.2049,   2.2049,   2.12566,  2.16423,  2.11398,   2.2049,   2.2049,   2.2049,   2.2049,
    0.5015,  0.44339, 0.631886, 0.516923, 0.595227,   0.5015,   0.5015,   0.5015,   0.55961, 0.371114, 0.486077, 0.407773,   0.5015,   0.5015,   0.5015,
    0.3528, 0.299973, 0.462123, 0.376339,  0.48417, 0.418721,   0.3528,   0.3528,  0.405627, 0.243477, 0.329261,  0.22143, 0.286879,   0.3528,   0.3528,
         0,        0,        0,        0,        0,        0,  0.34641,        0,         0,        0,        0,        0,        0, -0.34641,        0,
         0,        0,        0,        0,        0,        0,        0,  0.34641,         0,        0,        0,        0,        0,        0, -0.34641;

  //create matrix with predicted sigma points as columns
  MatrixXd Xsig_pred = MatrixXd(n_x, 2 * n_aug + 1);

  double delta_t = 0.1; //time diff in sec
/*******************************************************************************
 * Student part begin
 ******************************************************************************/

  //predict sigma points
  int n_aug_limit = 2 * n_aug + 1;
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
    Xsig_pred(0, i) = point_x_predicted;
    Xsig_pred(1, i) = point_y_predicted;
    Xsig_pred(2, i) = velocity_predicted;
    Xsig_pred(3, i) = yaw_predicted;
    Xsig_pred(4, i) = yaw_diff_predicted;

  }
    

/*******************************************************************************
 * Student part end
 ******************************************************************************/

  //print result
  std::cout << "Xsig_pred = " << std::endl << Xsig_pred << std::endl;

  //write result
  *Xsig_out = Xsig_pred;

}
