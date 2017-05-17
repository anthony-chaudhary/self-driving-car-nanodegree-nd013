#define _USE_MATH_DEFINES

#include "uWS/uWS.h"
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>
#include <stdlib.h>

using namespace std;

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }



// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::stringstream hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return std::stringstream();
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    std::stringstream tmp = std::stringstream();
    tmp.str(s.substr(b1, b2 - b1 + 1));
    return tmp;
  }
  return std::stringstream();
}


int main(int argc, const char *argv[])
{
  uWS::Hub h;

  PID pid ;
  PID pid_throttle ;
  // TODO: Initialize the pid variable.

    /*****************************************************************************
   *  Setup
   ****************************************************************************/

  double kp = 0.0 ;   
  double ki = 0.0 ;    
  double kd = 0.0 ;
  double speed_goal_local = 0.0 ;

  double throttle_kp = 0.0 ;   
  double throttle_ki = 0.0 ;    
  double throttle_kd = 0.0 ;

  if (argc == 8 ) {
    
    kp = strtod( argv[1], NULL ) ;
    ki = strtod( argv[2], NULL ) ;
    kd = strtod( argv[3], NULL ) ;

    speed_goal_local = strtod( argv[4], NULL ) ;

    throttle_kp = strtod( argv[5], NULL ) ;
    throttle_ki = strtod( argv[6], NULL ) ;
    throttle_kd = strtod( argv[7], NULL ) ;

  } else {

    cout << "Usage ./pid.exe kp ki kd speed_goal throttle_kp throttle_ki throttle_kd (Requires program + 7 parameter arguments)" << endl ;
    return -1 ;
  }

  //double kp = 1.6 ;   
  //double ki = 0.010 ;    
  //double kd = 55.0  

  pid.Init(kp, ki, kd) ;
  pid_throttle.Init(throttle_kp, throttle_ki, throttle_kd) ;
  pid_throttle.speed_goal = speed_goal_local ;


  h.onMessage([&pid_throttle, &pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data));
      if (s.str() != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
         
          /*****************************************************************************
           *  Twiddle
           ****************************************************************************/

          
          // put to true to use twiddle
          bool twiddleFlag = false ; 


          if (twiddleFlag == true && pid.twiddle_counter % 100 == 0) {

            vector<double> parameters;
            parameters = { pid.Kp, pid.Ki, pid.Kd} ;

            vector<double> tune_parameters;
            tune_parameters = { pid.tune_Kp, pid.tune_Ki, pid.tune_Kd };
            
            cout << "Iteration" << pid.twiddle_counter <<
            "\t best error" <<  pid.twiddle_best_error_ <<
            "\t parameters" << parameters[0] << parameters[1] << parameters[2] << endl ;


      		  for (int i = 0; i < 3; i++) {

      			  parameters[i] += tune_parameters[i] ;

              double twiddle_current_error ;
              
              pid.UpdateError(cte) ;
      			  twiddle_current_error =  pid.mean_squared_error_ ;

      			  if (twiddle_current_error < pid.twiddle_best_error_) {  //last update worked, keep going

      				  tune_parameters[i] *= 1.1 ;
      				  pid.twiddle_best_error_ = twiddle_current_error ;
      				  // exit, since increase worked, -> loop
      			  }
      			  else {

      				  parameters[i] -= 2 * tune_parameters[i] ;
      				  pid.UpdateError(cte) ;
                twiddle_current_error = pid.mean_squared_error_  ;

      				  // check if decrease worked, if not, do a "reset" on the parameters
      				  if (twiddle_current_error < pid.twiddle_best_error_) {

      					  pid.twiddle_best_error_ = twiddle_current_error ;
      					  tune_parameters[i] *= 1.1 ;
      				  }
      				  else {
      					  parameters[i] += tune_parameters[i] ;
      					  tune_parameters[i] *= .80 ;
      				  }
      			  }
            }

            pid.Kp = parameters[0] ;
            pid.Ki = parameters[1] ;
            pid.Kd = parameters[2] ;

            pid.tune_Kp = tune_parameters[0] ;
            pid.tune_Ki = tune_parameters[1] ;
            pid.tune_Kd = tune_parameters[2] ;

            cout << "P: " << pid.Kp << "\t P-tune: " << pid.tune_Kp << "\t P-tune / I-Tune " << 
            pid.tune_Kp / pid.tune_Ki << endl ;
            cout << "I: " << pid.Ki << "\t I-tune: " << pid.tune_Ki << "\t P-tune / D-Tune " <<
            pid.tune_Kp / pid.tune_Kd << endl ;
            cout << "D: " << pid.Kd << "\t D-tune: " << pid.tune_Kd << endl ;

            // restart simulation every x steps
            
            double raw_steer_value = pid.TotalError() ; ;
            double normalizer ;
            if (pid.Kp == 0.0) {
              normalizer = pid.tune_Kp ;
            } 
            else {
              normalizer = pid.Kp * 25 ;
            }
            double steer_value = raw_steer_value / normalizer ;

            // if (pid.twiddle_counter % 2000 == 0){ 

            if (steer_value > .25 ){ 
              
              std::string reset_msg = "42[\"reset\",{}]"; 
              ws.send(reset_msg.data(), reset_msg.length(), uWS::OpCode::TEXT); 

            }
            

    		  }
          
          pid.twiddle_counter += 1 ;

          
          /*****************************************************************************
           *  Proportional integral derivative controllers
           ****************************************************************************/
          
          double speed_difference ;
          speed_difference = fabs(pid_throttle.speed_goal - speed );

          pid_throttle.UpdateError(speed_difference) ;
          pid.UpdateError(cte) ;

          double throttle_speed = pid_throttle.TotalError() ;
          throttle_speed = fabs(throttle_speed) ;

          // Hard cap on speed instead of allowing PID to overshoot here.
          if (speed > pid_throttle.speed_goal){
            throttle_speed = 0 ;
          }

          // normalize 
          // speed is out of 0 to 1
          throttle_speed = throttle_speed / (10 * pid_throttle.speed_goal) ;
          double raw_steer_value = pid.TotalError() ;

          // normalize steering angle between 0 and 1:
          double normalizer ;
          // Assumes if Kp == 0 we are using tune function
          if (pid.Kp == 0.0) {
            normalizer = pid.tune_Kp ;
          } 
          // angles 0 to 1 (gets  converted to )
          else {
            normalizer = pid.Kp * 10;
          }

          double steer_value = raw_steer_value / normalizer ;
          
          // stats for speed
          pid.sum_speed += speed ;
          pid.mean_speed =  pid.sum_speed / pid.twiddle_counter ;

          if (pid.max_speed < speed) {
            pid.max_speed = speed ;
          }

          // debugging
          if (pid.counter_ % 100 == 0) {

            cout <<  "  mean_speed  " << pid.mean_speed << "\t max_speed " << pid.max_speed << endl ;

            cout << "CTE: " << cte << " Steering Value: " << steer_value << "  speed difference "
            << speed_difference << "  throttle_speed  " << throttle_speed << endl;
          }



          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle_speed;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          (ws).send(msg.data(), msg.length(), uWS::OpCode::TEXT);

          cout << "\t" << endl;
        }
      }
      else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        (ws).send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    (ws).close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen("0.0.0.0", port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}