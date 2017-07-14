#include <iostream>
#include <random>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <iterator>
#include "vehicle.h"

using namespace std;

class Road {
public:

	int update_width = 70;

  	string ego_rep = " *** ";

  	int ego_key = -1;

  	int num_lanes;

    vector<int> lane_speeds;

    int speed_limit;

    double density;

    int camera_center;

    map<int, Vehicle> vehicles;

    int vehicles_added = 0;

    /**
  	* Constructor
  	*/
  	Road(int speed_limit, double traffic_density, vector<int> lane_speeds);

  	/**
  	* Destructor
  	*/
  	virtual ~Road();

  	Vehicle get_ego();

  	void populate_traffic();

  	void advance();

  	void display();

  	void add_ego(int lane_num, int s, vector<int> config_data);

  	void cull();

};
