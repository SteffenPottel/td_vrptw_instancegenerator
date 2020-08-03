//---------------------------------------------------------------------------
#include "Grid.h"
#include "Point.h"
#include "PrioQueue.h"

#include <stdexcept>
#include <iostream>
//---------------------------------------------------------------------------
using namespace InstanceGenerator;

PrioQueue::PrioQueue(Point* point) {
	this->push(point);
}

bool PrioQueue::trivial() {
	bool trivial = false;

	if (this->queue.empty() ) trivial = true;

	return trivial;
}

int PrioQueue::find_index(Point* point) {
	int index = -1;
	int counter = 0;

	if (this->queue.empty() ) {
		index = 0;
	}
	else {
		for( std::list< std::pair< double, Point* > >::iterator it=this->queue.begin(); it != this->queue.end(); ++it ) {
			if (point->worth <= it->first && point == it->second) {
				index = counter;
				break;
			}
			++counter;
		}
		if (index == -1) index = this->queue.size();
	}

	return index;
}

void PrioQueue::push(Point* point) {
	int index;
	std::list< std::pair< double, Point* > >::iterator it=this->queue.begin();


	index = this->find_index(point);
	std::advance(it, index);
	this->queue.insert( it , std::make_pair(point->worth, point) );
}

Point* PrioQueue::pop() {
	Point* result = this->queue.front().second;
	this->queue.pop_front();

	return result;
}

void PrioQueue::kill(Point* point) {
	int removal_index = this->find_index(point);
	std::list< std::pair< double, Point* > >::iterator it=this->queue.begin();
	std::advance(it, removal_index);

    if (removal_index == this->queue.size() ) {
        throw std::logic_error("Queue removal: removal index not in queue.");
    }

	if (it->second != point) {
//std::cerr << it->first << "\t" << point->worth << std::endl;
//std::cerr << it->second->x << "\t" << point->x << std::endl;
		throw std::logic_error("Queue removal: wrongfully determined index.");
	}

	this->queue.erase( it );
}
