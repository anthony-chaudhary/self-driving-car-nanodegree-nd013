#ifndef MPC_H
#define MPC_H

#include <vector>
#include "Eigen-3.3/Eigen/Core"

using namespace std;

class MPC {
 public:

  double throttle ;
  double steering_angle ;

  /*
  const double coefficient_cost_ref_cte ;
  const double coefficient_cost_ref_epsi ;
  const double coefficient_cost_ref_v ;
  const double coefficient_cost_ref_val_throttle ;
  const double coefficient_cost_ref_val_steering ;
  const double coefficient_cost_ref_seq_throttle ;
  const double coefficient_cost_ref_seq_steering ;
  */

  MPC();

  virtual ~MPC();

  // Solve the model given an initial state and polynomial coefficients.
  // Return the first actuatotions.
  vector<double> Solve(Eigen::VectorXd state, Eigen::VectorXd coeffs);
};

#endif /* MPC_H */
