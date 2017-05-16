#include "PID.h"
#include <math.h>
#include <iostream>

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {

/*
where
Kp = proptional
ki = integral
kd = differential
cte = cross track error

*/

  previous_cte_      = 0.0 ;
  integral_cte_      = 0.0 ;
  difference_cte_    = 0.0 ;
  sum_square_error_  = 0.0 ;
  counter_			 = 0 ;

  sum_absolute_value_error_ = 0.0 ;

  twiddle_best_error_ = 999999.0 ;

  PID::Kp = Kp ;
  PID::Ki = Ki ;
  PID::Kd = Kd ;

  tune_Kp = .01 ;
  tune_Ki = .0001 ;
  tune_Kd = .01 ;
  twiddle_counter = 0 ;

  speed_goal = 65.0 ;

}

void PID::UpdateError(double cte) {

	counter_ += 1 ;

	current_cte_ = cte ;

	difference_cte_ = cte - previous_cte_ ;

    integral_cte_ += cte ;

	sum_absolute_value_error_ += fabs(cte) ;


	// 1.
	sum_square_error_ += cte * cte ;
	// 2.
	mean_squared_error_ = sum_square_error_ / counter_ ;


	if (counter_ % 10 == 0) {
		
		cout << "\t" << endl ;

		cout << "Counter\t" << counter_ <<
		"  current_cte_\t" << current_cte_ <<
		"  previous_cte_\t" << previous_cte_ <<
		"  sum_absolute_value_error_\t" << sum_absolute_value_error_ <<
		"  mean_squared_error_\t" << mean_squared_error_ << endl ;
	}

	previous_cte_ = cte ;

}

double PID::TotalError() {

	if (counter_ % 100 == 0) {

		cout << "P total error: " << - Kp * current_cte_ << endl ;
        cout << "I total error: " << - Kd * difference_cte_ << endl ;
        cout << "D total error: " << - Ki * integral_cte_ << endl ;

	}
	
	return  - Kp * current_cte_ - Kd * difference_cte_ - Ki * integral_cte_ ;
}

