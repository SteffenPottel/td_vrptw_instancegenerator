//---------------------------------------------------------------------------
#include "Point.h"
#include "Grid.h"
#include "Link.h"
#include "PWL.h"
#include "Waypoints.h"
#include "Instance.h"

#include <iostream>
#include <ios>
#include <fstream>
#include <thread>
#include <list>
#include <vector>
using std::cerr;
using std::endl;

//---------------------------------------------------------------------------
using namespace InstanceGenerator;

void usage() {
	std::cout << "Please, use the command 'executable file' 'input filename' 'output filename' 'number of initial points' 'maximal number of segments'." << endl;
}

int main (int argc, char* argv[]) {
    int constant = 1;
    int time_horizon;
    double scale;
    int num_of_points, num_of_segments;


	if (argc != 5) {
        usage();
        exit(1);
    }
	std::cout << "Instance Generator" << std::endl;

	num_of_points = std::stoi(argv[3]);
	num_of_segments = std::stoi(argv[4]);

	std::list<Point> hotspots;
	hotspots.push_back(Point(25,25,0.8));
	hotspots.push_back(Point(25,75,0.8));
	hotspots.push_back(Point(75,25,0.8));
	hotspots.push_back(Point(75,75,0.8));
///Generate Grid
	Grid grid1 = Grid(100,100);
	grid1.heatmap(hotspots, 1);
///Generate Instance
	Waypoints coordinates = Waypoints(argv[1]);
	time_horizon = coordinates.get_time_horizon(argv[1]);
	scale = float(time_horizon) / 1000;
	Instance ini = Instance();


///APPROACH WITH TD-A*
	for (std::vector<std::pair<std::string ,std::pair<int, int > > >::iterator it_out = coordinates.customers.begin(); it_out != coordinates.customers.end(); ++it_out) {
		for (std::vector<std::pair<std::string ,std::pair<int, int > > >::iterator it_in = coordinates.customers.begin(); it_in != coordinates.customers.end(); ++it_in) {
			if (it_out->second != it_in->second) {
				ini.td_get_activity(grid1, it_out->second, it_in->second, time_horizon, it_out->first + " " + it_in->first, scale, num_of_points, num_of_segments);
			}
		}
	}

	std::fstream file_handle;

	file_handle.open(argv[2], std::ios::out);

	for (std::list<std::string>::iterator it = ini.route_ids.begin(); it != ini.route_ids.end(); ++it) {
		ini.td_write_to_file(file_handle, *it);
	}

}

