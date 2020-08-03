//---------------------------------------------------------------------------
#include "Link.h"
#include "Grid.h"
#include "PWL.h"
#include "Point.h"
#include "Tools.h"

#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
using std::endl;
using std::cerr;
//---------------------------------------------------------------------------

/**
    Value assigned changed in order to allow evaluation on a grid different from the path-finding grid.
**/

using namespace InstanceGenerator;

Link::Link(std::vector<Point*>& computed_path, PWL& congestion_timer, double horizon, Grid& eva_grid) {
	path = computed_path;
	path_length();
	assign_values(horizon, congestion_timer, eva_grid);
}

void Link::path_length() {
	length = 0;
	int x_from, y_from, x_to, y_to;

	x_from = (*path.begin())->x;
	y_from = (*path.begin())->y;
	for (std::vector<Point*>::const_iterator it = path.begin(); it != path.end(); ++it) {
		x_to = (*it)->x;
		y_to = (*it)->y;
		length += std::sqrt( (x_to - x_from)*(x_to - x_from) + (y_to - y_from)*(y_to - y_from) );
		x_from = (*it)->x;
		y_from = (*it)->y;
	}
	return;
}

void Link::assign_values(double time_horizon, PWL& congestion_timer, Grid& eva_grid) {
	double timefactor;
	double update_time;
	double update_speed;
	double travel;
	double cons;
	int steps = 1000;
	double step_size = time_horizon / (float)steps;
	double current_time = 0;
	int len = path.size();
	int counter = 1;
	bool cut = false;

	while (current_time < time_horizon) {
		timefactor = congestion_timer.get_value(current_time);
		travel = 0;
		cons = 0;
		for (int i = 0; i < len-1; i++) {
			update_time = timefactor * ( eva_grid.move_costs_time( eva_grid.grid[path[i]->x][path[i]->y], eva_grid.grid[path[i+1]->x][path[i+1]->y] ) - (*path[i]).euclidean_distance(*path[i+1]) ) + (*path[i]).euclidean_distance(*path[i+1]);
			update_speed = timefactor * ( eva_grid.move_costs_speed( eva_grid.grid[path[i]->x][path[i]->y], eva_grid.grid[path[i+1]->x][path[i+1]->y] ) - 1 ) + 1;
			travel += update_time;
			cons += update_speed; /// Update the formula!!!
			if (current_time + travel >= time_horizon) {
                cut = true;
                break;
			}
			timefactor = congestion_timer.get_value(current_time + travel);
		}
		if (cut) break;
		travel_times.push_back(travel);
		consumption.push_back(cons);

		current_time += step_size;
		counter++;
	}
}

std::vector<std::pair<int, double>> Link::reduce_similar(std::vector<std::pair<int, double>>& values) {
	std::vector<std::pair<int, double>> result;
	double slope1, slope2, rel_slope;
	double threshold = 0.05;
	double eps = 0.001;

	result.push_back(values[0]);
	int length = values.size();
	int index = 1;
	while (index < length - 2) {
		slope1 = get_slope(result.back().first, result.back().second, values[index].first, values[index].second);
		slope2 = get_slope(values[index].first, values[index].second, values[index+1].first, values[index+1].second);
		if (slope1-slope2 < eps) {
			rel_slope = 0;
		}
		else if (slope1 < eps) {
			rel_slope = std::fabs(slope2);
		}
		else {
			rel_slope = std::fabs(slope1 - slope2) / slope1;
		}
		if (rel_slope < threshold) {
			index++;
		}
		else {
			result.push_back(values[index]);
			index++;
		}
	}
	result.push_back(values.back());
	return result;
}

std::vector<std::pair<int, double>> Link::reduce_curvature_cost(std::vector<std::pair<int, double>>& values, Grid& grid, std::pair<int, int>& source, std::pair<int, int>& target, double scale) {
	std::vector<std::pair<int, double>> result;
	double slope1, slope2, slope3;
	PWL congestion_timer = PWL(scale);

	result.push_back(values[0]);
	int length = values.size();
	int index = 1;
	while (index < length - 2) {
		slope1 = get_slope(result.back().first, result.back().second, values[index].first, values[index].second);
		slope2 = get_slope(values[index].first, values[index].second, values[index+1].first, values[index+1].second);
		slope3 = get_slope(values[index+1].first, values[index+1].second, values[index+2].first, values[index+2].second);

		if ( (slope2-slope1>0 && slope3-slope2>0) || (slope2-slope1<0 && slope3-slope2<0) ) {
			int arg = int(0.5*(values[index].first + values[index+1].first));
			grid.reset_path();
			grid.td_astar(grid.grid[source.first][source.second], grid.grid[target.first][target.second], congestion_timer, float(arg) );
			result.push_back(std::make_pair(arg, grid.grid[target.first][target.second].costs));
			index += 2;
		}
		else {
			result.push_back(values[index]);
			index++;
		}
	}
	result.push_back(values.back());

	return result;
}

std::vector<std::pair<int, double>> Link::reduce_curvature_consumption(std::vector<std::pair<int, double>>& values, Grid& grid, std::pair<int, int>& source, std::pair<int, int>& target, double scale) {
	std::vector<std::pair<int, double>> result;
	double slope1, slope2, slope3;
	PWL congestion_timer = PWL(scale);

	result.push_back(values[0]);
	int length = values.size();
	int index = 1;
	while (index < length - 2) {
		slope1 = get_slope(result.back().first, result.back().second, values[index].first, values[index].second);
		slope2 = get_slope(values[index].first, values[index].second, values[index+1].first, values[index+1].second);
		slope3 = get_slope(values[index+1].first, values[index+1].second, values[index+2].first, values[index+2].second);

		if ( (slope2-slope1>0 && slope3-slope2>0) || (slope2-slope1<0 && slope3-slope2<0) ) {
			int arg = int(0.5*(values[index].first + values[index+1].first));
			grid.reset_path();
			grid.td_astar(grid.grid[source.first][source.second], grid.grid[target.first][target.second], congestion_timer, float(arg) );
			result.push_back(std::make_pair(arg, grid.grid[target.first][target.second].consumption));
			index += 2;
		}
		else {
			result.push_back(values[index]);
			index++;
		}
	}
	result.push_back(values.back());

	return result;
}
