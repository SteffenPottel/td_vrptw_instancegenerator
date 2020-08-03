//---------------------------------------------------------------------------
#ifndef GRIDH
#define GRIDH

#include <list>
#include <vector>
#include "Point.h"
#include "PWL.h"
//---------------------------------------------------------------------------
namespace InstanceGenerator{

	/**
		The Grid class is the base layout for instances.
	**/
	class Grid {
	private:
		int height, width;
	public:
		std::vector< std::vector < Point > > grid;

	public:
		Grid(int height, int width);
		~Grid() {};

		void populate();
		bool on_grid(int x, int y);
		std::vector<Point*> neighbors(Point& point);
		double move_costs_speed(Point& source, Point& target);
		double move_costs_time(Point& source, Point& target);
		void heatmap(std::list<Point>& hotspots, double timefactor);
		void reset_path();

		///for completeness we add the trace_path function
		std::vector<Point*> trace_path(Point* point);

		void astar(Point& source, Point& target);
		void td_astar(Point& source, Point& target, PWL& timer, double start_time);

	};
}
#endif
