#ifndef PID_H
#define PID_H

class PID {
public:
  /*
  * Errors
  */
  // _ at end to indicate class variable

  double current_cte_ ;

  double integral_cte_ ;
  double difference_cte_ ;
  double previous_cte_ ;

  double sum_square_error_ ;
  double sum_absolute_value_error_ ;
  double mean_squared_error_ ;

  double twiddle_best_error_ ;

  int counter_ ;

  /*
  * Coefficients
  */ 
  double Kp ;
  double Ki ;
  double Kd ;

  double tune_Kp ;
  double tune_Ki ;
  double tune_Kd ;
  int twiddle_counter ;

  float speed_goal ;

  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();


};

#endif /* PID_H */
