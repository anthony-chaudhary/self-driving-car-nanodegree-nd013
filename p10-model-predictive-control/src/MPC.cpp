#include "MPC.h"
#include <cppad/cppad.hpp>
#include <cppad/ipopt/solve.hpp>
#include "Eigen-3.3/Eigen/Core"

using CppAD::AD;
using namespace std ;

size_t N = 16;
double dt = .016;

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
const double Lf = 2.67;

double ref_cte = 0;
double ref_epsi = 0;
double ref_v = 64;

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

/****************************************
 * cost hyperparamters
 ****************************************/
// TO DO refector to optional command line arguments

const double coeff_cost_ref_cte           = 320;
const double coeff_cost_ref_epsi          = 64;
const double coeff_cost_ref_v             = 1;
const double coeff_cost_ref_val_throttle  = 16;
const double coeff_cost_ref_val_steering  = 160;
const double coeff_cost_ref_seq_throttle  = 32;
const double coeff_cost_ref_seq_steering  = 6400;

class FG_eval {

 public:

  // Fitted polynomial coefficients
  Eigen::VectorXd coeffs;
  FG_eval( Eigen::VectorXd coeffs) { this->coeffs = coeffs; }

  typedef CPPAD_TESTVECTOR(AD<double>) ADvector;
  void operator()( ADvector& fg, const ADvector& vars) {

    // `fg` is a vector containing the cost and constraints.
    // `vars` is a vector containing the variable values (state & actuators).
    // The cost is stored is the first element of `fg`.
    // Any additions to the cost should be added to `fg[0]`.
    // Reference State Cost

    /****************************************
     * Cost vectors
     ****************************************/

    fg[0] = 0 ;

    // Cost for reference state
    for (int t = 0;   t < N;  t++) {
      fg[0] += coeff_cost_ref_cte   * CppAD::pow( vars[cte_start + t]   - ref_cte,  2) ;
      fg[0] += coeff_cost_ref_epsi  * CppAD::pow( vars[epsi_start + t]  - ref_epsi,  2) ;
      fg[0] += coeff_cost_ref_v     * CppAD::pow( vars[v_start + t]     - ref_v,  2) ;
    }

    // Reduce actuators values
    for (int t = 0;   t < (N - 1);    t++) {
      fg[0] += coeff_cost_ref_val_steering * CppAD::pow( vars[delta_start + t],  2) ;
      fg[0] += coeff_cost_ref_val_throttle * CppAD::pow( vars[a_start     + t],  2) ;
    }

    // Reduce gap between actuations (sequence)
    for (int t = 0;   t < (N - 2);   t++) {

      // (t + 1) - (t)
      fg[0] += coeff_cost_ref_seq_steering * CppAD::pow( vars[delta_start + t + 1] - vars[delta_start + t],  2) ;
      fg[0] += coeff_cost_ref_seq_throttle * CppAD::pow( vars[a_start     + t + 1] - vars[a_start + t],  2) ;
    }

    /****************************************
     * Initial constraints
     ****************************************/

    // We add 1 to each of the starting indices due to cost being located at
    // index 0 of `fg`.
    // This bumps up the position of all the other values.
    fg[1 + x_start]     = vars[x_start];
    fg[1 + y_start]     = vars[y_start];
    fg[1 + psi_start]   = vars[psi_start];
    fg[1 + v_start]     = vars[v_start];
    fg[1 + cte_start]   = vars[cte_start];
    fg[1 + epsi_start]  = vars[epsi_start];

    /****************************************
     * Kinematic model
     ****************************************/

    for (int i = 0; i < N - 1; i++) {

      // State time t + 1
      AD<double> x_t1     = vars[x_start    + i + 1] ;
      AD<double> y_t1     = vars[y_start    + i + 1] ;
      AD<double> psi_t1   = vars[psi_start  + i + 1] ;
      AD<double> v_t1     = vars[v_start    + i + 1] ;
      AD<double> cte_t1   = vars[cte_start  + i + 1] ;
      AD<double> e_psi_t1 = vars[epsi_start + i + 1] ;

      // State at time t
      AD<double> x_t0     = vars[x_start    + i] ;
      AD<double> y_t0     = vars[y_start    + i] ;
      AD<double> v_t0     = vars[v_start    + i] ;
      AD<double> psi_t0   = vars[psi_start  + i] ;
      AD<double> cte_t0   = vars[cte_start  + i] ;
      AD<double> e_psi_t0 = vars[epsi_start + i] ;

      AD<double> delta_t0 = vars[delta_start + i] ;
      AD<double> a_t0     = vars[a_start      + i] ;

      // for 3rd degree polynomial
      AD<double> f_t0 = coeffs[0] + 
                        coeffs[1] * x_t0 + 
                        coeffs[2] * x_t0 * x_t0 ;
      
      // rate of cahnge of f0
      AD<double> f_t0_rate_of_change = coeffs[1] +
                                       coeffs[2] * x_t0 * x_t0 ;       
       
      AD<double> psides_t0 = CppAD::atan( f_t0_rate_of_change) ;


      // Here's `x` to get you started.
      // The idea here is to constraint this value to be 0.
      //
      // NOTE: The use of `AD<double>` and use of `CppAD`!
      // This is also CppAD can compute derivatives and pass
      // these to the solver.

      fg[2 + x_start + i]    = x_t1   - (x_t0 + v_t0 * CppAD::cos(psi_t0) * dt ) ;
      fg[2 + y_start + i]    = y_t1   - (y_t0 + v_t0 * CppAD::sin(psi_t0) * dt ) ;

      fg[2 + psi_start + i]  = psi_t1 - (psi_t0 + v_t0 * delta_t0 / Lf * dt ) ;
      //fg[2 + psi_start + i]   = (psi_t1 + v_t1 * delta_t0 / Lf * dt ) ;
      fg[2 + v_start + i]    = v_t1   - (v_t0 - a_t0 * dt) ;

      fg[2 + cte_start + i]  = cte_t1 - ( (f_t0 - y_t0) + 
                              (v_t0 * CppAD::sin(e_psi_t0) * dt) ) ;
      
      fg[2 + epsi_start + i] = e_psi_t1 - ( ( psi_t0 + psides_t0) + 
                               v_t0 * delta_t0 / Lf * dt ) ;

    }
  }
};

//
// MPC class definition implementation.
//
MPC::MPC() {}
MPC::~MPC() {}

vector<double> MPC::Solve(Eigen::VectorXd state, Eigen::VectorXd coeffs) {
  bool ok = true;
  size_t i;
  typedef CPPAD_TESTVECTOR(double) Dvector;

  // Set the number of model variables (includes both states and inputs).
  // For example: If the state is a 4 element vector, the actuators is a 2
  // element vector and there are 10 timesteps. The number of variables is:
  // 4 * 10 + 2 * 9

  double x    = state[0];
  double y    = state[1];
  double psi  = state[2];
  double v    = state[3];
  double cte  = state[4];
  double epsi = state[5];


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
  vars[psi_start] = psi;
  vars[v_start] = v;
  vars[cte_start] = cte;
  vars[epsi_start] = epsi;

  Dvector vars_lowerbound(n_vars);
  Dvector vars_upperbound(n_vars);

  /****************************************
   * Limits
   ****************************************/

  // Set lower and upper limits for variables.
  // Set all non-actuators upper and lowerlimits
  // to the max negative and positive values.
  for (int i = 0; i < delta_start; i++) {
    vars_lowerbound[i] = -1.0e19;
    vars_upperbound[i] =  1.0e19;
  }

  // The upper and lower limits of delta are set to -25 and 25
  // degrees (values in radians).
  for (int i = delta_start; i < a_start; i++) {
    vars_lowerbound[i] = -0.436332;
    vars_upperbound[i] =  0.436332;
  }

  // Acceleration/decceleration upper and lower limits.
  for (int i = a_start; i < n_vars; i++) {
    vars_lowerbound[i] = -1.;
    vars_upperbound[i] =  1.;
  }

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
  constraints_lowerbound[psi_start]   = psi;
  constraints_lowerbound[v_start]     = v;
  constraints_lowerbound[cte_start]   = cte;
  constraints_lowerbound[epsi_start]  = epsi;

  constraints_upperbound[x_start]     = x;
  constraints_upperbound[y_start]     = y;
  constraints_upperbound[psi_start]   = psi;
  constraints_upperbound[v_start]     = v;
  constraints_upperbound[cte_start]   = cte;
  constraints_upperbound[epsi_start]  = epsi;



  // object that computes objective and constraints
  FG_eval fg_eval(coeffs);

  // options for IPOPT solver
  std::string options;
  // Uncomment this if you'd like more print information
  options += "Integer print_level  0\n";
  // NOTE: Setting sparse to true allows the solver to take advantage
  // of sparse routines, this makes the computation MUCH FASTER. If you
  // can uncomment 1 of these and see if it makes a difference or not but
  // if you uncomment both the computation time should go up in orders of
  // magnitude.
  options += "Sparse  true        forward\n";
  options += "Sparse  true        reverse\n";
  // NOTE: Currently the solver has a maximum time limit of 0.5 seconds.
  // Change this as you see fit.
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

  // store steering and throttle variables in class
  // TODO rename all "delta" -> steering_angle and "a" -> throttle
  // for clarity
  this->steering_angle = solution.x[ delta_start ] ;
  this->throttle       = solution.x[ a_start ] ;

  cout << "steering_angle" << solution.x[ delta_start ] << 
  "throttle" << solution.x[ a_start ] << endl;

  vector<double> results ;
  results.push_back(solution.x[psi_start + 1]) ;
  // cout << "psi results " << results[psi_start + 1] << "at index:" << psi_start + 1 << endl ;
  results.push_back(solution.x[v_start + 1]) ;
  results.push_back(solution.x[cte_start + 1]) ;
  results.push_back(solution.x[epsi_start + 1]) ;

  results.push_back(solution.x[delta_start + 1]) ;
  results.push_back(solution.x[a_start + 1]) ;

  for (int i = 0 ;  i < N - 1  ;  i++) {
    results.push_back(solution.x[x_start + 1 + i]) ;
    results.push_back(solution.x[y_start + 1 + i]) ;
    // cout << "results x " << results[x_start + 1 + i] << endl ;
    // cout << "results y " << results[y_start + 1 + i] << endl ;
  }

  // cout << "results[0] " << results[0] << "results[1]" << results[1] << endl ;

  return results;
}
