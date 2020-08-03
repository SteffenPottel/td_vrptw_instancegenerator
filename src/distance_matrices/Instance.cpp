//---------------------------------------------------------------------------
#include "Instance.h"

#include "PWL.h"
#include "Grid.h"
#include "Point.h"
#include "Link.h"

#include <map>
#include <list>
#include <string>
#include <ios>
#include <iostream>
#include <fstream>
#include <stdexcept>
//-----------------------------------------------------
using namespace InstanceGenerator;

void Instance::td_get_activity(Grid& conggrid, std::pair<int, int>& source, std::pair<int, int>& target, double time_horizon, std::string route_id, double scale, int points_num, int segments_num) {
	PWL congestion_timer = PWL(scale);
	Link td_link = Link();
	int arg = 0;
	int step = int(time_horizon / (points_num - 1) );

	this->route_ids.push_back(route_id);
	///Generate the initial number of points
	while (arg <= time_horizon) {
		conggrid.reset_path();
		conggrid.td_astar(conggrid.grid[source.first][source.second], conggrid.grid[target.first][target.second], congestion_timer, float(arg) );

		td_link.travel_pairs.push_back(std::make_pair(arg , conggrid.grid[target.first][target.second].costs ));
		td_link.consumption_pairs.push_back(std::make_pair(arg, conggrid.grid[target.first][target.second].consumption));

		if (conggrid.grid[target.first][target.second].costs < 0) {
            std::cerr << "A* cost: " << conggrid.grid[target.first][target.second].costs << std::endl;
		}

		arg += step;
	}
	///Reduce points to sufficient number
	while (td_link.travel_pairs.size() > segments_num + 1) {
		td_link.travel_pairs = td_link.reduce_similar(td_link.travel_pairs);
		td_link.travel_pairs = td_link.reduce_curvature_cost(td_link.travel_pairs, conggrid, source, target, scale);
	}
	while (td_link.consumption_pairs.size() > segments_num + 1) {
		td_link.consumption_pairs = td_link.reduce_similar(td_link.consumption_pairs);
		td_link.consumption_pairs = td_link.reduce_curvature_consumption(td_link.consumption_pairs, conggrid, source, target, scale);
	}
	///Generate the PWL
	PWL td_times = PWL(td_link.travel_pairs, segments_num);
	td_times.valid_travel();
	//td_times.show();
	this->td_travel.insert(std::pair<std::string, PWL>(route_id, td_times));

	PWL td_consumption = PWL(td_link.consumption_pairs, segments_num);
	td_consumption.valid_consumption();
	//td_consumption.show();
	this->td_consume.insert(std::pair<std::string, PWL>(route_id, td_consumption));
}

void Instance::td_write_to_file(std::fstream& file_handle, std::string activity_id) {
	file_handle << "Link" << "\n";
	//write ID
	file_handle << activity_id << "\n";
	//number of segments for travel times
	file_handle << td_travel[activity_id].segments.size() << "\n";
	//congested travel times
	for (std::vector<Segment>::iterator it = td_travel[activity_id].segments.begin(); it != td_travel[activity_id].segments.end(); ++it) {
		file_handle << it->start << "\t" << it->ending << "\t" << it->slope << "\t" << it->intercept << "\t";
	}
	file_handle << "\n";
	//number of segments for consumption
	file_handle << td_consume[activity_id].segments.size() << "\n";
	//congested consumption
	for (std::vector<Segment>::iterator it = td_consume[activity_id].segments.begin(); it != td_consume[activity_id].segments.end(); ++it) {
		file_handle << it->start << "\t" << it->ending << "\t" << it->slope << "\t" << it->intercept << "\t";
	}
	file_handle << "\n";
}

