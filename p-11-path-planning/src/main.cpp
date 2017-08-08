#include <fstream>
#include "uWS/uWS.h"  // if WINDOWS
// IF LINUX
//#include <uWS/uWS.h>

#include <thread>
#include <vector>
#include <random>
#include "Eigen-3.3/Eigen/Core"
#include "Eigen-3.3/Eigen/QR"
#include "json.hpp"
#include "path.h"
#include "spline.h"
//#include "MPC.h"

#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

using namespace std;
using json = nlohmann::json;
default_random_engine			generator2;

stringstream hasData(string s) {
	auto found_null = s.find("null");
	auto b1 = s.find_first_of("[");
	auto b2 = s.find_last_of("]");
	if (found_null != string::npos) {
		return stringstream();
	}
	else if (b1 != string::npos && b2 != string::npos) {
		stringstream tmp = stringstream();
		tmp.str(s.substr(b1, b2 - b1 + 1));
		return tmp;
	}
	return stringstream();
}

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
	uWS::Hub h;

	//MPC mpc;
	//mpc.Init(hyper_parameters) ;

	// Load up map values for waypoint's x,y,s and d normalized normal vectors
	vector<double> map_waypoints_x;
	vector<double> map_waypoints_y;
	vector<double> map_waypoints_s;
	vector<double> map_waypoints_dx;
	vector<double> map_waypoints_dy;

	// Waypoint map to read from
	string map_file_ = "../data/highway_map.csv";  // CHANGE BACK to ../ after debugging
												// The max s value before wrapping around the track back to 0
	double max_s = 6945.554;

	ifstream in_map_(map_file_.c_str(), ifstream::in);

	string line;
	while (getline(in_map_, line)) {
		istringstream iss(line);
		double x, y;
		double s, d_x, d_y;
		iss >> x;
		iss >> y;
		iss >> s;
		iss >> d_x;
		iss >> d_y;
		map_waypoints_x.push_back(x);
		map_waypoints_y.push_back(y);
		map_waypoints_s.push_back(s);
		map_waypoints_dx.push_back(d_x);
		map_waypoints_dy.push_back(d_y);
	}

	/****************************************
	* 0. Initialize
	****************************************/
	path path;
	path.init();

	path.start_time = chrono::high_resolution_clock::now();
	// path.mpc_clock = chrono::high_resolution_clock::now() + 10000ms;

	tk::spline spline_x, spline_y;
	spline_x.set_points(map_waypoints_s, map_waypoints_x);
	spline_y.set_points(map_waypoints_s, map_waypoints_y);

	path::MAP *MAP = new path::MAP;

	// refine path with spline.
	int spline_samples = 12000;
	for (size_t i = 0; i < spline_samples; ++i) {
		MAP->waypoints_x_upsampled.push_back(spline_x(i));
		MAP->waypoints_y_upsampled.push_back(spline_y(i));
		MAP->waypoints_s_upsampled.push_back(i);
	}

	// IF different version of uwebsockts replace all "ws" with "(*ws)"!


	path::X_Y X_Y_, X_Y_2;

	h.onMessage([&](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {

		// "42" at the start of the message means there's a websocket message event.
		if (length && length > 2 && data[0] == '4' && data[1] == '2') {

			auto s = hasData(string(data));
			if (s.str() != "") {
				auto j = json::parse(s);
				string event = j[0].get<string>();
				if (event == "telemetry") { // j[1] is the data JSON object

					const double car_x = j[1]["x"];
					const double car_y = j[1]["y"];
					const double car_s = j[1]["s"];
					const double car_d = j[1]["d"];
					const double car_yaw = j[1]["yaw"];
					const double car_speed = j[1]["speed"];
					const auto previous_path_x = j[1]["previous_path_x"];
					const auto previous_path_y = j[1]["previous_path_y"];
					const double end_path_s = j[1]["end_path_s"];
					const double end_path_d = j[1]["end_path_d"];
					const auto sensor_fusion = j[1]["sensor_fusion"];

					json msgJson;

					path.current_time = chrono::high_resolution_clock::now();
					auto time_difference = chrono::duration_cast<std::chrono::milliseconds>(path.current_time - path.start_time).count();


					// cout << "previous_path_size " << previous_path_x.size() << endl;

					// cout << time_difference << endl;
					// cout << chrono::high_resolution_clock::to_time_t(chrono::high_resolution_clock::now()) << endl;

					if (time_difference > 1000) {

						// cout <<  "time_difference " << time_difference << endl;

						// 0. set clock for next round
						path.start_time = chrono::high_resolution_clock::now();


						// 1. Merge previous path and update car state
						auto Previous_path = path.merge_previous_path(MAP, previous_path_x,
							previous_path_y, car_yaw, car_s, car_d, end_path_s, end_path_d);

						// 2. Update vehicles with sensor fusion readings
						path.sensor_fusion_predict(sensor_fusion);

						// 3. Update our car's state (pending removal of arguments here as doing update at 1. now.0
						path.update_our_car_state(MAP, car_x, car_y, Previous_path.s, Previous_path.d, car_yaw, car_speed);

						// 4. Generate trajectory
						auto trajectory = path.trajectory_generation();

						// 5. Build trajectory using time
						auto S_D_ = path.build_trajectory(trajectory);

						// 6. Convert to X and Y and append previous path
						X_Y_ = path.convert_new_path_to_X_Y_and_merge(MAP, S_D_, Previous_path);

						
						//cout << X_Y_.X.size() << endl;

						msgJson["next_x"] = X_Y_.X;
						msgJson["next_y"] = X_Y_.Y;

			
					} 

				else {
					//cout << "Using previous path\n " << endl;
					msgJson["next_x"] = previous_path_x;
					msgJson["next_y"] = previous_path_y;
				}
								

				this_thread::sleep_for(chrono::milliseconds(100));
				
				auto msg = "42[\"control\"," + msgJson.dump() + "]";
				//std::cout << msg << std::endl;

				
				ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
				
				}
			}
			else {
				// Manual driving
				std::string msg = "42[\"manual\",{}]";
				ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
			}
		}
	});

	// needed to compile
	h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data,
		size_t, size_t) {
		const std::string s = "<h1>Hello world!</h1>";
		if (req.getUrl().valueLength == 1) {
			res->end(s.data(), s.length());
		}
		else {
			res->end(nullptr, 0);
		}
	});

	h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
		std::cout << "Connected!!!" << std::endl;
	});

	h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code,
		char *message, size_t length) {
		ws.close();
		std::cout << "Disconnected" << std::endl;
	});

	int port = 4567;
	if (h.listen("0.0.0.0", port)) {
		std::cout << "Listening to port " << port << std::endl;
	}
	else {
		std::cerr << "Failed to listen to port" << std::endl;
		return -1;
	}
	h.run();
}















































































