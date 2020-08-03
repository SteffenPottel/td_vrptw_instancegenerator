//---------------------------------------------------------------------------
#ifndef WAYPOINTSH
#define WAYPOINTSH

#include <vector>
#include <string>
//---------------------------------------------------------------------------
namespace InstanceGenerator{

	/**
		The Waypoints class
	**/
	class Waypoints {
	public:
		std::vector<std::pair< std::string, std::pair<int, int> > > customers;
	public:
		Waypoints(std::string filename);
		~Waypoints() {};

		void read_data(std::string filename);
		int get_time_horizon(std::string filename);
	};
}
#endif
