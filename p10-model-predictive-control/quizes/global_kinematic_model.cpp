// In this quiz you'll implement the global kinematic model.
#include <cmath>
#include <iostream>
#include "Eigen\Dense"

using namespace std ;
//
// Helper functions
//
double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

const double Lf = 2;

// TODO: Implement the global kinematic model.
// Return the next state.
//
// NOTE: state is [x, y, psi, v]
// NOTE: actuators is [delta, a]
Eigen::VectorXd globalKinematic(Eigen::VectorXd state,
                                Eigen::VectorXd actuators, double dt) {
  
  Eigen::VectorXd next_state(state.size());

  //TODO complete the next_state calculation ...

  double x, y, psi, v ;
  double delta, alpha ;
  double x_t1, y_t1, psi_t1, v_t1 ;

  x   = state[0] ;
  y   = state[1] ;
  psi = state [2] ;
  v   = state [3] ;

  delta = rad2deg( actuators[0] );
  alpha = actuators[1] ;

  // dt == dt from function arguments

  x_t1    = x + v * cos( psi ) * dt ;
  y_t1    = y + v * sin( psi ) * dt ;
  psi_t1  = deg2rad ( rad2deg( psi ) + ( v / Lf ) * delta * dt );
  v_t1    = v + alpha * dt ;

  // psi_t1  = deg2rad(psi_t1) ;

  // cout << "psi " << psi<< endl;
  // cout << "psi + ( v / Lf ) * delta * dt " << psi + ( v / Lf ) * delta * dt << endl;
  // cout << "deg2rad ( rad2deg( psi ) + ( v / Lf ) * delta * dt ) " << deg2rad ( rad2deg( psi ) + ( v / Lf ) * delta * dt ) << endl;
  // cout << "rad2deg( psi ) + ( v / Lf ) * delta * dt " << rad2deg( psi ) + ( v / Lf ) * delta * dt << endl;

  next_state  <<  x_t1, y_t1, psi_t1, v_t1 ;
  
  return next_state;
}

int main() {
  // [x, y, psi, v]
  Eigen::VectorXd state(4);
  // [delta, v]
  Eigen::VectorXd actuators(2);

  state << 0, 0, deg2rad(45), 1;
  actuators << deg2rad(5), 1;

  // std::cout << state << std::endl;

  Eigen::VectorXd next_state = globalKinematic(state, actuators, 0.3);

  std::cout << next_state << std::endl;
}