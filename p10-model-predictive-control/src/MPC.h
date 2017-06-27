#ifndef MPC_H
#define MPC_H

#include <vector>
#include "Eigen-3.3/Eigen/Core"

using namespace std;

extern double coeff_cost_ref_cte ;
extern double coeff_cost_ref_epsi ;
extern double coeff_cost_ref_v ;
extern double coeff_cost_ref_val_throttle ;
extern double coeff_cost_ref_val_steering ;
extern double coeff_cost_ref_seq_throttle ;
extern double coeff_cost_ref_seq_steering ;


class MPC {
 public:

  double throttle ;
  double steering_angle ;

  MPC();

  virtual ~MPC();

  // Solve the model given an initial state and polynomial coefficients.
  // Return the first actuatotions.
  void Init(vector<double> v) ;

  vector<double> Solve(Eigen::VectorXd state, Eigen::VectorXd coeffs);
};

#endif /* MPC_H */
