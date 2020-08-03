//---------------------------------------------------------------------------
#ifndef LINKH
#define LINKH

#include "Point.h"
#include "PWL.h"
#include "Grid.h"

#include <vector>
#include <list>
//---------------------------------------------------------------------------
namespace InstanceGenerator{

	/**
		The Link class captures the properties of a path
		between two points on the grid and admits the computation
		of travel times and consumption.
	**/
	class Link {
	public:
		std::vector<Point*> path;
		double length;
		std::vector<double> travel_times, consumption;
		std::vector<std::pair<int, double>> travel_pairs, consumption_pairs;
	public:
		Link() {};
		Link(std::vector<Point*>& computed_path, PWL& congestion_timer, double horizon, Grid& grid);
		~Link() {};

		void path_length();
		void assign_values(double time_horizon, PWL& congestion_timer, Grid& grid);
		std::vector<std::pair<int, double>> reduce_similar(std::vector<std::pair<int, double>>& values);
		std::vector<std::pair<int, double>> reduce_curvature_cost(std::vector<std::pair<int, double>>& values, Grid& grid, std::pair<int, int>& source, std::pair<int, int>& target, double scale);
		std::vector<std::pair<int, double>> reduce_curvature_consumption(std::vector<std::pair<int, double>>& values, Grid& grid, std::pair<int, int>& source, std::pair<int, int>& target, double scale);
	};
}
#endif
