//---------------------------------------------------------------------------
#ifndef PRIOQUEUEH
#define PRIOQUEUEH

#include "Point.h"
#include "Grid.h"

#include <list>
//---------------------------------------------------------------------------
namespace InstanceGenerator{

	/**
		The Priority Queue class is a simple implementation
		of a weighted queue.
	**/
	class PrioQueue {
	public:
		std::list< std::pair< double, Point* > > queue;
	public:
		PrioQueue(Point* initial_element);
		~PrioQueue() {};

		bool trivial();
		void push(Point* point);
		Point* pop();
		void kill(Point* point);
		int find_index(Point* point);

	};
}
#endif
