#ifndef HYBRID_BREADTH_FIRST_H_
#define HYBRID_BREADTH_FIRST_H_

#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

class HBF {
public:

	int NUM_THETA_CELLS = 90;
	double SPEED = 1.45;
	double LENGTH = 0.5;

	struct maze_s {
	
		int g;	// iteration
		double x;
		double y;
		double theta;
	};

	struct maze_path {
	
		vector< vector< vector<maze_s> > > closed;
		vector< vector< vector<maze_s> > > came_from;
		maze_s final;

	};


	/**
  	* Constructor
  	*/
 	HBF();

	/**
 	* Destructor
 	*/
 	virtual ~HBF();

 	
 	int theta_to_stack_number(double theta);

  	int idx(double float_num);

  	vector<maze_s> expand(maze_s state);

  	maze_path search(vector< vector<int> > grid, vector<double> start, vector<int> goal);

  	vector<maze_s> reconstruct_path(vector< vector< vector<maze_s> > > came_from, vector<double> start, HBF::maze_s final);
  	

};

#endif
