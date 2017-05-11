#define _USE_MATH_DEFINES

#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>


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


int main()
{
  uWS::Hub h;

  PID pid;
  // TODO: Initialize the pid variable.
  

  h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> *ws, char *data, size_t length, uWS::OpCode opCode) {
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
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */

          /*****************************************************************************
           *  Setup
           ****************************************************************************/

          // TODO REVIEW THIS NONE RIGHT YET
          double steer_value  = 0 ;
          double err          = 0 ;
          double prev_cte     = 0 ;
          double int_cte      = 0 ;
          double diff_cte     = 0 ;

          /*****************************************************************************
           *  Twiddle
           ****************************************************************************/

		  vector<double> parameters;
		  parameters = { 5.0, 5.0, 0.1 } ;

		  vector<double> tune_parameters;
		  tune_parameters = { 1.0, 1.0, 1.0 };
          // double sum_tune_parameters      = tune_parameters.sum() ;

		  double best_err = 99999999.0;
          int counter      = 0 ;

          cout << "Iteration" << counter <<
          "\t best error" <<  best_err <<
          "\t parameters" << parameters[0] << parameters[1] << parameters[2] << endl ;

		  for (int i = 0; i < 3; i++) {

			  parameters[i] += tune_parameters[i];
			  double current_error = 1;    // TODO set current error UpdateError(cte)?

			  // print(sum_parameters)
			  if (current_error < best_err) {  //last update worked, keep going

				  tune_parameters[i] *= 1.1;
				  best_err = current_error;
				  // exit, since increase worked, -> loop
			  }
			  else {

				  parameters[i] -= 2 * tune_parameters[i];
				  current_error = 1;    // TODO set current error

				  // check if decrease worked, if not, do a "reset" on the parameters
				  if (current_error < best_err) {

					  best_err = current_error;
					  tune_parameters[i] *= 1.1;
				  }
				  else {
					  parameters[i] += tune_parameters[i];
					  tune_parameters[i] *= .95;
				  }
			  }
		  }

          /*****************************************************************************
           *  Proportional integral derivative controller
           ****************************************************************************/
            
          diff_cte = cte - prev_cte ;
          int_cte += cte ;

          steer_value = -parameters[0] * cte - parameters[1] * diff_cte - parameters[2] * int_cte ;
          
          prev_cte = cte ;    // update CTE for next loop
          counter += 1 ;


          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.3;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          (*ws).send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      }
      else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        (*ws).send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {
    (*ws).close();
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