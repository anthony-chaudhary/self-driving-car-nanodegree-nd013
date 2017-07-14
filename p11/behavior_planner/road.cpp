#include <iostream>
#include "road.h"
#include "vehicle.h"
#include <iostream>
#include <math.h>
#include <map>
#include <string>
#include <iterator>
#include <algorithm>


/**
 * Initializes Road
 */
Road::Road(int speed_limit, double traffic_density, vector<int> lane_speeds) {

    this->num_lanes = lane_speeds.size();
    this->lane_speeds = lane_speeds;
    this->speed_limit = speed_limit;
    this->density = traffic_density;
    this->camera_center = this->update_width/2;

}

Road::~Road() {}

Vehicle Road::get_ego() {
	
	return this->vehicles.find(this->ego_key)->second;
}

void Road::populate_traffic() {
	
	int start_s = max(this->camera_center - (this->update_width/2), 0);
	for (int l = 0; l < this->num_lanes; l++)
	{
		int lane_speed = this->lane_speeds[l];
		bool vehicle_just_added = false;
		for(int s = start_s; s < start_s+this->update_width; s++)
		{
			
			if(vehicle_just_added)
			{
				vehicle_just_added = false;
			}
			if(((double) rand() / (RAND_MAX)) < this->density)
			{
				
				Vehicle vehicle = Vehicle(l,s,lane_speed,0);
				vehicle.state = "CS";
				this->vehicles_added += 1;
				this->vehicles.insert(std::pair<int,Vehicle>(vehicles_added,vehicle));
				vehicle_just_added = true;
			}
		}
	}
	
}

void Road::advance() {
	
	map<int ,vector<vector<int> > > predictions;

	map<int, Vehicle>::iterator it = this->vehicles.begin();
    while(it != this->vehicles.end())
    {
        int v_id = it->first;
        vector<vector<int> > preds = it->second.generate_predictions(10);
        predictions[v_id] = preds;
        it++;
    }
	it = this->vehicles.begin();
	while(it != this->vehicles.end())
    {
    	int v_id = it->first;
        if(v_id == ego_key)
        {
        	it->second.update_state(predictions);
        	it->second.realize_state(predictions);
        }
        it->second.increment(1);
        
        it++;
    }
    
}

void Road::display() {

	Vehicle ego = this->vehicles.find(this->ego_key)->second;
	int s = ego.s;
	string state = ego.state;

    this->camera_center = max(s, this->update_width/2);
    int s_min = max(this->camera_center - this->update_width/2, 0);
    int s_max = s_min + this->update_width;

    vector<vector<string> > road;

    for(int i = 0; i < this->update_width; i++)
    {
    	vector<string> road_lane;
    	for(int ln = 0; ln < this->num_lanes; ln++)
    	{
    		road_lane.push_back("     ");
    	}
    	road.push_back(road_lane);

    }

    map<int, Vehicle>::iterator it = this->vehicles.begin();
    while(it != this->vehicles.end())
    {

        int v_id = it->first;
        Vehicle v = it->second;

        if(s_min <= v.s && v.s < s_max)
        {
        	string marker = "";
        	if(v_id == this->ego_key)
        	{
        		marker = this->ego_rep;
        	}
        	else
        	{
        		
        		stringstream oss;
        		stringstream buffer;
        		buffer << " ";
        		oss << v_id;
        		for(int buffer_i = oss.str().length(); buffer_i < 3; buffer_i++)
        		{
        		    buffer << "0";
        		
        		}
        		buffer << oss.str() << " ";
        		marker = buffer.str();
        	}
        	road[int(v.s - s_min)][int(v.lane)] = marker;
        }
        it++;
    }
    ostringstream oss;
    int i = s_min;
    for(int lj = 0; lj < road.size(); lj++)
    {
        if(i%20 ==0)
    	{
    	    stringstream buffer;
    	    stringstream dis;
        	dis << i;
        	for(int buffer_i = dis.str().length(); buffer_i < 3; buffer_i++)
        	{
        		 buffer << "0";
        	}
    	    
    		oss << buffer.str() << dis.str() << " - ";
    	}
    	else
    	{
    		oss << "      ";
    	}          
    	i++;
    	for(int li = 0; li < road[0].size(); li++)
    	{
    		oss << "|" << road[lj][li];
    	}
    	oss << "|";
    	oss << "\n";
    }
    oss << "+=============================+" << endl;
    cout << oss.str();

}

void Road::add_ego(int lane_num, int s, vector<int> config_data) {
	
	map<int, Vehicle>::iterator it = this->vehicles.begin();
    while(it != this->vehicles.end())
    {
    	int v_id = it->first;
        Vehicle v = it->second;
        if(v.lane == lane_num && v.s == s)
        {
        	this->vehicles.erase(v_id);
        }
        it++;
    }
    Vehicle ego = Vehicle(lane_num, s, this->lane_speeds[lane_num], 0);
    ego.configure(config_data);
    ego.state = "KL";
    this->vehicles.insert(std::pair<int,Vehicle>(ego_key,ego));
    
}

void Road::cull() {
	
	
	Vehicle ego = this->vehicles.find(this->ego_key)->second;
	int center_s = ego.s;
	set<vector<int>> claimed;

	map<int, Vehicle>::iterator it = this->vehicles.begin();
    while(it != this->vehicles.end())
    {
    	int v_id = it->first;
        Vehicle v = it->second;
        vector<int> claim_pair = {v.lane,v.s};
        claimed.insert(claim_pair);
        it++;
    }
    it = this->vehicles.begin();
    while(it != this->vehicles.end())
    {
    	int v_id = it->first;
        Vehicle v = it->second;
    	if( (v.s > (center_s + this->update_width / 2) ) || (v.s < (center_s - this->update_width / 2) ) )
    	{
    		try {
    			claimed.erase({v.lane,v.s});
    		}
    		catch (const exception& e) {
    			continue;
    		}
    		this->vehicles.erase(v_id);

    		bool placed = false;
    		while(!placed) {
    			int lane_num = rand() % this->num_lanes;
    			int ds = rand() % 14 + (this->update_width/2-15);
    			if(lane_num > this->num_lanes/2)
    			{
    				ds*=-1;
    			}
    			int s = center_s + ds;
    			if(claimed.find({lane_num,s}) != claimed.end())
    			{
    				placed = true;
    				int speed = lane_speeds[lane_num];
    				Vehicle vehicle = Vehicle(lane_num, s, speed, 0);
    				this->vehicles_added++;
    				this->vehicles.insert(std::pair<int,Vehicle>(vehicles_added,vehicle));
    				cout << "adding vehicle "<< this->vehicles_added << " at lane " << lane_num << " with s=" << s << endl;
    			}

    		}
    	}
    	it++;
    }
    

}


