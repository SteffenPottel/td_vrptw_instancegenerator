//---------------------------------------------------------------------------
#ifndef POINTH
#define POINTH

#include <list>
//---------------------------------------------------------------------------
namespace InstanceGenerator{

	/**
		The Point class is a container of node information.
	**/
	class Point {
	public:
		int x, y;
		double weight, time, speed; // weight is the congestion factor
		int prev_x, prev_y;
		double worth, estimate, costs, consumption;
	public:
		Point(int x, int y, double weight);
		~Point() {};

		double euclidean_distance(Point& target) ;
		double normal_distribution(Point& source) ;
		double assign_weight(std::list<Point>& hotspots) ;
		double travel_speed(double timefactor) ;
		double travel_time(double timefactor) ;
		double step_consumption(Point& target, double timefactor);
		void astar_reset() ;

		friend bool operator==(Point& p1, Point& p2) {
			bool equal = false;

			if ( p1.x == p2.x && p1.y == p2.y ) equal = true;

			return equal;
		}

	};
}
#endif
