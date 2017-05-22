#include "MPC.h"
#include <math.h>
#include "COIN-OR-1.8.0-win32-msvc12/COIN-OR/win32-msvc12/include/cppad/cppad.hpp"
#include "COIN-OR-1.8.0-win32-msvc12/COIN-OR/win32-msvc12/include/cppad/ipopt/solve.hpp"
#include "Eigen-3.3/Eigen/Core"
#include "Eigen-3.3/Eigen/QR"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

using CppAD::AD;

size_t N  = 25 ;
double dt = .1 ;
double T  = N * dt ;

// This value assumes the model presented in the classroom is used.
//
// It was obtained by measuring the radius formed by running the vehicle in the
// simulator around in a circle with a constant steering angle and velocity on a
// flat terrain.
//
// Lf was tuned until the the radius formed by the simulating the model
// presented in the classroom matched the previous radius.
//
// This is the length from front to center of gravity that has a similar radius.
const double Lf = 2.67;

double ref_cte = 0;
double ref_epsi = 0;
double ref_v = 40;

// The solver takes all the state variables and actuator
// variables in a singular vector. Thus, we should to establish
// when one variable starts and another ends to make our lifes easier.
size_t x_start      = 0 ;
size_t y_start      = x_start     + N ;
size_t psi_start    = y_start     + N ;
size_t v_start      = psi_start   + N ;
size_t cte_start    = v_start     + N ;
size_t epsi_start   = cte_start   + N ;
size_t delta_start  = epsi_start  + N ;
size_t a_start      = delta_start + N - 1 ;

class FG_eval {

 public:

  Eigen::VectorXd coeffs;

  // Coefficients of the fitted polynomial.
  FG_eval(Eigen::VectorXd coeffs) { this->coeffs = coeffs; }

  typedef CPPAD_TESTVECTOR(AD<double>) ADvector;

  void operator()(ADvector& fg, const ADvector& vars) {
    // `fg` is a vector containing the cost and constraints.
    // `vars` is a vector containing the variable values (state & actuators).
    // The cost is stored is the first element of `fg`.
    // Any additions to the cost should be added to `fg[0]`.

    // Reference State Cost
    // Define the cost related the reference state and
    // any anything you think may be beneficial.

    fg[0] = 0 ;

    // bind new variable cost to fg[0] for ease of reading
    double &cost = fg[0] ;

    // Cost for reference state
    for (int t = 0;   t < N;  t++) {
      cost += CppAD::pow( vars[cte_start + t] - ref_cte,  2) ;
      cost += CppAD::pow( vars[epsi_start + t] - ref_epsi,  2) ;
      cost += CppAD::pow( vars[v_start + t] - ref_v,  2) ;
    }


    // Reduce actuators
    for (int t = 0;   t < (N - 1);    t++) {
      cost += CppAD::pow( vars[delta_start + t],  2) ;
      cost += CppAD::pow( vars[a_start + t],  2) ;
    }

    // Reduce gap between actuations
    for (int t = 0;   t < (N - 2);   t++) {

      // (t + 1) - (t)
      cost += CppAD::pow( vars[delta_start + t + 1] - vars[delta_start + t],  2) ;
      cost += CppAD::pow( vars[a_start + t + 1] - vars[a_start + t],  2) ;
    }

    //
    // Setup Constraints
    // NOTE: In this section you'll setup the model constraints.

    // Initial constraints
    //
    // We add 1 to each of the starting indices due to cost being located at
    // index 0 of `fg`.
    // This bumps up the position of all the other values.
    fg[1 + x_start]     = vars[x_start];
    fg[1 + y_start]     = vars[y_start];
    fg[1 + psi_start]   = vars[psi_start];
    fg[1 + v_start]     = vars[v_start];
    fg[1 + cte_start]   = vars[cte_start];
    fg[1 + epsi_start]  = vars[epsi_start];

    // The rest of the constraints
    for (int i = 0; i < N - 1; i++) {

      // State time t + 1
      AD<double> x_t1     = vars[x_start + i + 1] ;
      AD<double> y_t1     = vars[y_start + i + 1] ;
      AD>double> psi_t1   = vars[psi_start + i + 1] ;
      AD>double> v_t1     = vars[v1_start + i + 1] ;
      AD>double> cte_t1   = vars[cte_start + i + 1] ;
      AD>double> e_psi_t1 = vars[epsi_start + i + 1] ;

      // State at time t
      AD<double> x_t0     = vars[x_start + i] ;
      AD<double> y_t0     = vars[y_start + i] ;
      AD<double> v_t0     = vars[v_start + i] ;
      AD<double> psi_t0   = vars[psi_start + i] ;
      AD<double> cte_t0   = vars[cte_start + i] ;
      AD<double> e_psi_t0 = vars[e_psi_start + i] ;

      AD<double> delta_t0 = vars[delta_start + i] ;
      AD<double> a_t0     = vars[a_start + i] ;

      AD<double> f_t0 = coeffs[0] + coeffs[1] * x_t0 ;
      AD<double> psides_t0 = CppAD::atan( coeffs[1] ) ;
      // Here's `x` to get you started.
      // The idea here is to constraint this value to be 0.
      //
      // NOTE: The use of `AD<double>` and use of `CppAD`!
      // This is also CppAD can compute derivatives and pass
      // these to the solver.

      // Setup the rest of the model constraints

      // 2 for space for cost vectors and x_t1?
      fg[2 + x_start + i]   = x_t1   - (x_t0 + v_t0 * CppAD::cos(psi_t0) * dt ) ;
      fg[2 + y_start + i]   = y_t1   - (y_t0 + v_t0 * CppAD::sin(psi_t0) * dt ) ;
      fg[2 + psi_start + i] = psi_t1 - (psi_t0 + v_t0 * delta_t0 / Lf * dt ) ;
      fg[2 + v_start + i]   = v_t1  - (v_t0 - a_t0 * dt) ;
      fg[2 + cte_start + i] = 
            cte_t1 - ( (f_t0 - y_t0) + (v_t0 * CppAD::sin(e_psi_t0) * dt) ) ;
      fg[2 + epsi_start + i] =
            epsi1 - ( ( psi_t0 + psides_t0) + v_start * delta_t0 / Lf * dt ) ;

    }
  }
};

//
// MPC class definition
//

MPC::MPC() {}
MPC::~MPC() {}

vector<double> MPC::Solve(Eigen::VectorXd x0, Eigen::VectorXd coeffs) {
  size_t i;
  typedef CPPAD_TESTVECTOR(double) Dvector;

  double x = x0[0];
  double y = x0[1];
  double psi = x0[2];
  double v = x0[3];
  double cte = x0[4];
  double epsi = x0[5];

  // number of independent variables
  // N timesteps == N - 1 actuations
  size_t n_vars = N * 6 + (N - 1) * 2;
  // Number of constraints
  size_t n_constraints = N * 6;

  // Initial value of the independent variables.
  // Should be 0 except for the initial values.
  Dvector vars(n_vars);
  for (int i = 0; i < n_vars; i++) {
    vars[i] = 0.0;
  }
  // Set the initial variable values
  vars[x_start] = x;
  vars[y_start] = y;
  vars[psi_start] = psi;
  vars[v_start] = v;
  vars[cte_start] = cte;
  vars[epsi_start] = epsi;

  // Lower and upper limits for x
  Dvector vars_lowerbound(n_vars);
  Dvector vars_upperbound(n_vars);

  // Set all non-actuators upper and lowerlimits
  // to the max negative and positive values.
  for (int i = 0; i < delta_start; i++) {
    vars_lowerbound[i] = -1.0e19;
    vars_upperbound[i] = 1.0e19;
  }

  // The upper and lower limits of delta are set to -25 and 25
  // degrees (values in radians).
  // NOTE: Feel free to change this to something else.
  for (int i = delta_start; i < a_start; i++) {
    vars_lowerbound[i] = -0.436332;
    vars_upperbound[i] = 0.436332;
  }

  // Acceleration/decceleration upper and lower limits.
  // NOTE: Feel free to change this to something else.
  for (int i = a_start; i < n_vars; i++) {
    vars_lowerbound[i] = -1.0;
    vars_upperbound[i] = 1.0;
  }

  // Lower and upper limits for constraints
  // All of these should be 0 except the initial
  // state indices.
  Dvector constraints_lowerbound(n_constraints);
  Dvector constraints_upperbound(n_constraints);
  for (int i = 0; i < n_constraints; i++) {
    constraints_lowerbound[i] = 0;
    constraints_upperbound[i] = 0;
  }
  constraints_lowerbound[x_start] = x;
  constraints_lowerbound[y_start] = y;
  constraints_lowerbound[psi_start] = psi;
  constraints_lowerbound[v_start] = v;
  constraints_lowerbound[cte_start] = cte;
  constraints_lowerbound[epsi_start] = epsi;

  constraints_upperbound[x_start] = x;
  constraints_upperbound[y_start] = y;
  constraints_upperbound[psi_start] = psi;
  constraints_upperbound[v_start] = v;
  constraints_upperbound[cte_start] = cte;
  constraints_upperbound[epsi_start] = epsi;

  // Object that computes objective and constraints
  FG_eval fg_eval(coeffs);

  // options
  std::string options;
  options += "Integer print_level  0\n";
  options += "Sparse  true        forward\n";
  options += "Sparse  true        reverse\n";
  options += "Numeric max_cpu_time          0.05\n";

  // place to return solution
  CppAD::ipopt::solve_result<Dvector> solution;

  // solve the problem
  CppAD::ipopt::solve<Dvector, FG_eval>(
      options, vars, vars_lowerbound, vars_upperbound, constraints_lowerbound,
      constraints_upperbound, fg_eval, solution);

  //
  // Check some of the solution values
  //
  bool ok = true;
  ok &= solution.status == CppAD::ipopt::solve_result<Dvector>::success;

  auto cost = solution.obj_value;
  std::cout << "Cost " << cost << std::endl;
  return {solution.x[x_start + 1],   solution.x[y_start + 1],
          solution.x[psi_start + 1], solution.x[v_start + 1],
          solution.x[cte_start + 1], solution.x[epsi_start + 1],
          solution.x[delta_start],   solution.x[a_start]};
}

//
// Helper functions to fit and evaluate polynomials.
//

// Evaluate a polynomial.
double polyeval(Eigen::VectorXd coeffs, double x) {
  double result = 0.0;
  for (int i = 0; i < coeffs.size(); i++) {
    result += coeffs[i] * pow(x, i);
  }
  return result;
}

// Fit a polynomial.
// Adapted from
// https://github.com/JuliaMath/Polynomials.jl/blob/master/src/Polynomials.jl#L676-L716
Eigen::VectorXd polyfit(Eigen::VectorXd xvals, Eigen::VectorXd yvals,
                        int order) {
  assert(xvals.size() == yvals.size());
  assert(order >= 1 && order <= xvals.size() - 1);
  Eigen::MatrixXd A(xvals.size(), order + 1);

  for (int i = 0; i < xvals.size(); i++) {
    A(i, 0) = 1.0;
  }

  for (int j = 0; j < xvals.size(); j++) {
    for (int i = 0; i < order; i++) {
      A(j, i + 1) = A(j, i) * xvals(j);
    }
  }

  auto Q = A.householderQr();
  auto result = Q.solve(yvals);
  return result;
}

int main() {
  
  MPC mpc;
  int iters = 50;

  Eigen::VectorXd ptsx(2);
  Eigen::VectorXd ptsy(2);
  ptsx << -100, 100;
  ptsy << -1, -1;

  // it a polynomial to the above x and y coordinates
  auto coeffs = polyfit(ptsx, ptsy, 1)

  // NOTE: free feel to play around with these
  double x = -1;
  double y = 10;
  double psi = 0;
  double v = 10;
  // calculate the cross track error
  // f(x) - y
  // f(x) == polyeval(polyfit(ptsx, ptsy, 1), 0)
  double cte = polyeval(coeffs, 0) - y ;
  // calculate the orientation error
  double epsi = - atan(coeffs[1]) ;

  Eigen::VectorXd state(6);
  state << x, y, psi, v, cte, epsi;

  std::vector<double> x_vals = {state[0]};
  std::vector<double> y_vals = {state[1]};
  std::vector<double> psi_vals = {state[2]};
  std::vector<double> v_vals = {state[3]};
  std::vector<double> cte_vals = {state[4]};
  std::vector<double> epsi_vals = {state[5]};
  std::vector<double> delta_vals = {};
  std::vector<double> a_vals = {};

  for (size_t i = 0; i < iters; i++) {
    std::cout << "Iteration " << i << std::endl;

    auto vars = mpc.Solve(state, coeffs);

    x_vals.push_back(vars[0]);
    y_vals.push_back(vars[1]);
    psi_vals.push_back(vars[2]);
    v_vals.push_back(vars[3]);
    cte_vals.push_back(vars[4]);
    epsi_vals.push_back(vars[5]);

    delta_vals.push_back(vars[6]);
    a_vals.push_back(vars[7]);

    state << vars[0], vars[1], vars[2], vars[3], vars[4], vars[5];
  }

  // Plot values
  // NOTE: feel free to play around with this.
  // It's useful for debugging!
  plt::subplot(3, 1, 1);
  plt::title("CTE");
  plt::plot(cte_vals);
  plt::subplot(3, 1, 2);
  plt::title("Delta (Radians)");
  plt::plot(delta_vals);
  plt::subplot(3, 1, 3);
  plt::title("Velocity");
  plt::plot(v_vals);

  plt::show();
}