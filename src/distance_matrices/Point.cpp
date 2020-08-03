//---------------------------------------------------------------------------
#include "Point.h"

#include <math.h>
#include <cfloat>
#include <list>
#include <stdexcept>
using std::list;
//---------------------------------------------------------------------------
using namespace InstanceGenerator;

Point::Point(int x, int y, double weight) : x(x), y(y), weight(weight) {
	prev_x = -1;
	prev_y = -1;
	worth = DBL_MAX;
	estimate = DBL_MAX;
	costs = DBL_MAX;
}

double Point::euclidean_distance(Point& target) {
	double result = sqrt( (x - target.x)*(x - target.x) + (y - target.y)*(y - target.y) );
	return result;
}

double Point::normal_distribution(Point& source) {
	double sigma = 15;
	double distance = Point::euclidean_distance(source);
	double result = source.weight * exp( - 0.5 * distance*distance / (sigma*sigma) );
	return result;
}

double Point::assign_weight(list<Point>& hotspots) {
	double result = 0;
	for ( list<Point>::iterator it = hotspots.begin(); it != hotspots.end(); it++ ) {
		result += Point::normal_distribution( *it );
	}
	return result;
}

double Point::travel_speed(double timefactor) {
	double result = 1 - weight * timefactor;
	if (result < 0.1) {
        throw std::logic_error("Congestion is too intense!");
	}
	return result;
}

double Point::travel_time(double timefactor) {
	double result;
	if (speed == 0) {
		Point::travel_speed(timefactor);
		Point::travel_time(timefactor);
	}
	else {
		result = 1 / speed;
	}
	return result;
}

void Point::astar_reset() {
	prev_x = -1;
	prev_y = -1;
	worth = DBL_MAX;
	estimate = DBL_MAX;
	costs = DBL_MAX;
}

double Point::step_consumption(Point& target, double timefactor) {
	double result;
	double speed;
	double vertex_x, vertex_y, abs_factor;
	/// squared potential in vertex form
	speed = timefactor * ( 0.5 * (this->speed + target.speed) - 1 ) + 1;
	abs_factor = 1.388;
	vertex_x = 0.4;
	vertex_y = 0.5;
	///consumption per step computed by
	/// y = a * (x - h) + k
	result = abs_factor * (speed - vertex_x)*(speed - vertex_x) + vertex_y;
	return result;
}
