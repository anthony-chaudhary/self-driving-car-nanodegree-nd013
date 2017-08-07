#include "MPC.h"
#include <cppad/cppad.hpp>
#include <cppad/ipopt/solve.hpp>
#include "Eigen-3.3/Eigen/Core"

using CppAD::AD;
using namespace std ;


size_t N = 10;
double dt = .02;

// This value assumes the model presented in the classroom is used.
//
// It was obtained by measuring the radius formed by running the vehicle in the
// simulator around in a circle with a constant steering angle and velocity on a
// flat terrain.
//
// Lf was tuned until the the radius formed by the simulating the model
// presented in the classroom matched the previous radius.
//
// This is the length from front to CoG that has a similar radius.
const double Lf = 2.9;

double ref_cte = 0;
double ref_epsi = 0;
double ref_v = 20;   

// A. Solver takes 1 vector.
//  This is to create an index to access variables in that fector
size_t x_start      = 0 ;
size_t y_start      = x_start     + N ;
size_t psi_start    = y_start     + N ;
size_t v_start      = psi_start   + N ;
size_t cte_start    = v_start     + N ;
size_t epsi_start   = cte_start   + N ;
size_t delta_start  = epsi_start  + N ;
size_t a_start      = delta_start + N - 1 ;
/// end A.

double coeff_cost_ref_cte = 1;
double coeff_cost_ref_epsi = 1;
double coeff_cost_ref_v = 1;
double coeff_cost_ref_val_throttle = 1;
double coeff_cost_ref_val_steering = 1;
double coeff_cost_ref_seq_throttle = 1;
double coeff_cost_ref_seq_steering = 1;

//
// MPC class definition implementation.
//
MPC::MPC() {}
MPC::~MPC() {}

void MPC::Init(vector<double> coeff_hyper_parameters ){

  /****************************************
   * cost hyperparamters
   ****************************************/

  coeff_cost_ref_cte           = coeff_hyper_parameters[0];
  coeff_cost_ref_epsi          = coeff_hyper_parameters[1];
  coeff_cost_ref_v             = coeff_hyper_parameters[2];
  coeff_cost_ref_val_throttle  = coeff_hyper_parameters[3];
  coeff_cost_ref_val_steering  = coeff_hyper_parameters[4];
  coeff_cost_ref_seq_throttle  = coeff_hyper_parameters[5];
  coeff_cost_ref_seq_steering  = coeff_hyper_parameters[6];
}



class FG_eval {

 public:

  // Fitted polynomial coefficients
  Eigen::VectorXd coeffs;
  FG_eval( Eigen::VectorXd coeffs) { this->coeffs = coeffs; }

  typedef CPPAD_TESTVECTOR(AD<double>) ADvector;
  void operator()( ADvector& fg, const ADvector& vars) {

        
    fg[0] = 0 ;

    // Cost for reference state
    for (int t = 0;   t < N;  t++) {
      fg[0] += coeff_cost_ref_cte   * CppAD::pow( vars[cte_start + t]   - ref_cte,  2) ;
      fg[0] += coeff_cost_ref_epsi  * CppAD::pow( vars[epsi_start + t]  - ref_epsi,  2) ;
      fg[0] += coeff_cost_ref_v     * CppAD::pow( vars[v_start + t]     - ref_v,  2) ;
    }
      /****************************************
     * Initial constraints
     ****************************************/

  
    fg[1 + x_start]     = vars[x_start];
    fg[1 + y_start]     = vars[y_start];
    fg[1 + cte_start]   = vars[cte_start];
    
    /****************************************
     * Kinematic model
     ****************************************/

    for (int i = 0; i < N - 1; i++) {

      // State time t + 1
      AD<double> x_t1     = vars[x_start    + i + 1] ;
      AD<double> y_t1     = vars[y_start    + i + 1] ;
      AD<double> cte_t1   = vars[cte_start  + i + 1] ;

      // State at time t
      AD<double> x_t0     = vars[x_start    + i] ;
      AD<double> y_t0     = vars[y_start    + i] ;
      AD<double> cte_t0   = vars[cte_start  + i] ;
      
  
      // for 3rd degree polynomial
      AD<double> f_t0 = coeffs[0] + 
                        coeffs[1] * x_t0 + 
                        coeffs[2] * x_t0 * x_t0 +
                        coeffs[3] * (x_t0 * x_t0 * x_t0) ;
      
      // rate of cahnge of f0
      AD<double> f_t0_rate_of_change = coeffs[1] +
                                       (2 * coeffs[2] * x_t0) +
                                       (3 * coeffs[3] * (x_t0 * x_t0) );       
       
      AD<double> psides_t0 = CppAD::atan( f_t0_rate_of_change) ;


      fg[2 + x_start + i]    = x_t1   - (x_t0 * dt ) ;
      fg[2 + y_start + i]    = y_t1   - (y_t0 * dt ) ;

      fg[2 + cte_start + i]  = cte_t1 - ( (f_t0 - y_t0) );

    }
  }
};




vector<double> MPC::Solve(Eigen::VectorXd state, Eigen::VectorXd coeffs) {
  bool ok = true;
  size_t i;
  typedef CPPAD_TESTVECTOR(double) Dvector;

  double x    = state[0];
  double y    = state[1];
  double cte  = state[2];
  

  size_t n_vars = N * 6 + (N - 1) * 2;;
  // Set the number of constraints
  size_t n_constraints = N * 6;

  // Initial value of the independent variables.
  // SHOULD BE 0 besides initial state.
  Dvector vars(n_vars);
  for (int i = 0; i < n_vars; i++) {
    vars[i] = 0;
  }

  // Set the initial variable values
  vars[x_start] = x;
  vars[y_start] = y;
  vars[cte_start] = cte;

  Dvector vars_lowerbound(n_vars);
  Dvector vars_upperbound(n_vars);

  /****************************************
   * Limits
   ****************************************/


  // Lower and upper limits for the constraints
  // Should be 0 besides initial state.
  Dvector constraints_lowerbound(n_constraints);
  Dvector constraints_upperbound(n_constraints);
  for (int i = 0; i < n_constraints; i++) {
    constraints_lowerbound[i] = 0;
    constraints_upperbound[i] = 0;
  }

  constraints_lowerbound[x_start]     = x;
  constraints_lowerbound[y_start]     = y;
  constraints_lowerbound[cte_start]   = cte;

  constraints_upperbound[x_start]     = x;
  constraints_upperbound[y_start]     = y;
  constraints_upperbound[cte_start]   = cte;
  
    // object that computes objective and constraints
  FG_eval fg_eval(coeffs);

  // options for IPOPT solver
  std::string options;
  // Uncomment this if you'd like more print information
  options += "Integer print_level  0\n";
  options += "Sparse  true        forward\n";
  options += "Sparse  true        reverse\n";
  options += "Numeric max_cpu_time          0.5\n";

  // place to return solution
  CppAD::ipopt::solve_result<Dvector> solution;

  // solve the problem
  CppAD::ipopt::solve<Dvector, FG_eval>( options, vars, vars_lowerbound, 
                                         vars_upperbound, constraints_lowerbound,
                                         constraints_upperbound, fg_eval, solution);

  // Check some of the solution values
  ok &= solution.status == CppAD::ipopt::solve_result<Dvector>::success;

  // Cost
  auto cost_final = solution.obj_value;
  std::cout << "Cost " << cost_final << std::endl;

  /****************************************
   * Store values
   ****************************************/

  vector<double> results ;
  

  for (int i = 0 ;  i < N - 1  ;  i++) {
    results.push_back(solution.x[x_start + 1 + i]) ;
    results.push_back(solution.x[y_start + 1 + i]) ;
   
  }

  return results;
}
