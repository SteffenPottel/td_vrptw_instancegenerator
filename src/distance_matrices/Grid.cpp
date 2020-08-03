//---------------------------------------------------------------------------
#include "Grid.h"
#include "Point.h"
#include "PrioQueue.h"

#include <stdexcept>
#include <math.h>
#include <list>
#include <vector>
#include <algorithm>
using std::list;
#include <queue>
#include <iostream>
using std::cerr;
using std::endl;
//---------------------------------------------------------------------------
using namespace InstanceGenerator;

Grid::Grid(int height, int width) : height(height), width(width) {
	for (int i=0; i < width; i++) {
		std::vector<Point> temp_vec;
		for (int j=0; j < height; j++) {
            Point point = Point(i,j,0);
			temp_vec.push_back( point );
		}
		grid.push_back(temp_vec);
	}
}

bool Grid::on_grid(int x, int y) {
	bool on = false;
	if (x >= 0 && x < width && y >= 0 && y < height) on=true;

	return on;
}

std::vector<Point*> Grid::neighbors(Point& point) {
	std::vector<Point*> result;
	int new_x, new_y;

	for (int i=0; i < 3; i++) {
		for (int j=0; j < 3; j++) {
			new_x = point.x + i - 1;
			new_y = point.y + j - 1;
			if (!(this->on_grid( new_x, new_y) ) || (new_x == point.x && new_y == point.y) ) {
				continue;
			}
			else {
				result.push_back( &Grid::grid[new_x][new_y] );
			}
		}
	}

	return result;
}

double Grid::move_costs_speed(Point& source, Point& target) {
	double result = 0;
	result = 0.5 * (source.speed + target.speed);

	return result;
}

double Grid::move_costs_time(Point& source, Point& target) {
	double result = 0;
	result = 0.5 * (source.time + target.time) * source.euclidean_distance(target);

	return result;
}

void Grid::heatmap(list<Point>& hotspots, double timefactor) {
	for ( std::vector< std::vector <Point> >::iterator it_row = Grid::grid.begin(); it_row != Grid::grid.end(); it_row++) {
		for ( std::vector<Point>::iterator it_col = it_row->begin(); it_col != it_row->end(); it_col++) {
			it_col->weight = it_col->assign_weight(hotspots);
			it_col->speed = it_col->travel_speed(timefactor);
			it_col->time = it_col->travel_time(timefactor);

		}
	}
}

void Grid::reset_path() {
	for ( std::vector< std::vector <Point> >::iterator it_row = Grid::grid.begin(); it_row != Grid::grid.end(); it_row++) {
		for ( std::vector<Point>::iterator it_col = it_row->begin(); it_col != it_row->end(); it_col++) {
			it_col->astar_reset();
		}
	}
}

std::vector<Point*> Grid::trace_path(Point* point) {
	std::vector<Point*> path;
	path.push_back(point);

	while (!(point->prev_x == point->x && point->prev_y == point->y)) {
		point = &Grid::grid[point->prev_x][point->prev_y];
		path.push_back(point);
	}
    ///Reverse the traced path for right order!
	std::reverse(path.begin(), path.end());

	return path;
}

void Grid::astar(Point& source, Point& target) {
	double new_costs;
	Point* current_node;
	std::vector<Point*> current_neighbors;

	///Security checks
	if (!(this->on_grid(source.x, source.y) ) ) {
		throw std::invalid_argument("A*: Source is not on grid!");
	}

	if (!(this->on_grid(target.x, target.y) ) ) {
		throw std::invalid_argument("A*: Target is not on grid!");
	}

	if (source == target) {
		throw std::logic_error("A*: Source and target are identical --> redundant computation!");
	}

	///Book-keeping
	bool visited[100][100] = {false};

	///set the source in the grid
	source.worth = 0;
	source.estimate = 0;
	source.costs = 0;
	source.prev_x = source.x;
	source.prev_y = source.y;

	///set the open container with priority for choosing the next element
	PrioQueue boundary = PrioQueue(&source);
	///start searching
	while( !boundary.trivial() ) {
		current_node = boundary.pop();
		visited[current_node->x][current_node->y] = true;

		///break condition
		if (current_node->x == target.x && current_node->y == target.y) break;

		///evaluate new candidates
		current_neighbors = this->neighbors(*current_node);
		///iterate over all neighbors
		for ( std::vector<Point*>::iterator it = current_neighbors.begin(); it != current_neighbors.end(); ++it ) {
			///avoid redundant computations
			if ( visited[(*it)->x][(*it)->y] ) continue;
			///update the bounary
			new_costs = current_node->costs + this->move_costs_time(*current_node, *(*it));
			///first, if it is not in the boundary already
			if ( (*it)->prev_x == -1 && (*it)->prev_y == -1 ) {
				(*it)->costs = new_costs;
				(*it)->estimate = (*it)->euclidean_distance(target);
				(*it)->worth = (*it)->costs + (*it)->estimate;
				(*it)->prev_x = current_node->x;
				(*it)->prev_y = current_node->y;
				boundary.push(*it);
			}
			else if (new_costs < (*it)->costs) {
				boundary.kill(*it);
				(*it)->costs = new_costs;
				(*it)->estimate = (*it)->euclidean_distance(target);
				(*it)->worth = (*it)->costs + (*it)->estimate;
				(*it)->prev_x = current_node->x;
				(*it)->prev_y = current_node->y;
				boundary.push(*it);
			}
			else {
				continue;
			}
		}
	}

}

void Grid::td_astar(Point& source, Point& target, PWL& congestion_timer, double start_time) {
	/**
		Same A* algorithm with local-in-time travel times.
		Computing the consumption likewise.
	**/
	double new_costs, step_cost, timefactor;
	Point* current_node;
	std::vector<Point*> current_neighbors;

	///Security checks
	if (!(this->on_grid(source.x, source.y) ) ) {
		throw std::invalid_argument("A*: Source is not on grid!");
	}

	if (!(this->on_grid(target.x, target.y) ) ) {
		throw std::invalid_argument("A*: Target is not on grid!");
	}

	if (source == target) {
		throw std::logic_error("A*: Source and target are identical --> redundant computation!");
	}

	///Book-keeping
	bool visited[100][100] = {false};

	///set the source in the grid
	source.worth = 0;
	source.estimate = 0;
	source.costs = 0;
	source.consumption = 0;
	source.prev_x = source.x;
	source.prev_y = source.y;

	///set the open container with priority for choosing the next element
	PrioQueue boundary = PrioQueue(&source);
	///start searching
	while( !boundary.trivial() ) {
		current_node = boundary.pop();
		visited[current_node->x][current_node->y] = true;

		///break condition
		if (current_node->x == target.x && current_node->y == target.y) break;

		///evaluate new candidates
		current_neighbors = this->neighbors(*current_node);
		///iterate over all neighbors
		for ( std::vector<Point*>::iterator it = current_neighbors.begin(); it != current_neighbors.end(); ++it ) {
			///avoid redundant computations
			if ( visited[(*it)->x][(*it)->y] ) continue;
			///update the bounary
			timefactor = congestion_timer.get_value(start_time + current_node->costs);
			step_cost = timefactor * ( this->move_costs_time(*current_node, *(*it)) - current_node->euclidean_distance(*(*it)) ) + current_node->euclidean_distance(*(*it));
			if (step_cost < 0) {
                std::cerr << "Negative Step: " << step_cost << "\t" << this->move_costs_time(*current_node, *(*it)) - current_node->euclidean_distance(*(*it)) << "\t" << timefactor << std::endl;
			}
			new_costs = current_node->costs + step_cost;
			///first, if it is not in the boundary already
			if ( (*it)->prev_x == -1 && (*it)->prev_y == -1 ) {
				(*it)->costs = new_costs;
				(*it)->consumption = current_node->consumption + current_node->step_consumption(*(*it), timefactor);
				(*it)->estimate = (*it)->euclidean_distance(target);
				(*it)->worth = (*it)->costs + (*it)->estimate;
				(*it)->prev_x = current_node->x;
				(*it)->prev_y = current_node->y;
				boundary.push(*it);
			}
			else if (new_costs < (*it)->costs) {
				boundary.kill(*it);
				(*it)->costs = new_costs;
				(*it)->consumption = current_node->consumption + current_node->step_consumption(*(*it), timefactor);
				(*it)->estimate = (*it)->euclidean_distance(target);
				(*it)->worth = (*it)->costs + (*it)->estimate;
				(*it)->prev_x = current_node->x;
				(*it)->prev_y = current_node->y;
				boundary.push(*it);
			}
			else {
				continue;
			}
		}
	}
}
