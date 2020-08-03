//---------------------------------------------------------------------------
#ifndef INSTANCEH
#define INSTANCEH

#include "PWL.h"
#include "Grid.h"
#include "Point.h"

#include <map>
#include <list>
#include <string>
#include <ios>
#include <fstream>
//---------------------------------------------------------------------------
namespace InstanceGenerator{

	/**
		The Instance class
	**/
	class Instance {
	public:
		std::list<std::string> route_ids;
		std::map<std::string, PWL> td_travel, td_consume;
	public:
		Instance() {};
		~Instance() {};

		void td_get_activity(Grid& conggrid, std::pair<int, int>& source, std::pair<int, int>& target, double time_horizon, std::string route_id, double scale, int points_num, int segments_num);
		void td_write_to_file(std::fstream& file_handle, std::string activity_id);
	};
}
#endif
