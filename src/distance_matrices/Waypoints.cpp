//---------------------------------------------------------------------------
#include "Waypoints.h"

#include <vector>
#include <string>
#include <fstream>
using std::ifstream;
#include <stdexcept>
#include <iostream>
//-----------------------------------------------------
using namespace InstanceGenerator;

Waypoints::Waypoints(std::string filename) {
    this->read_data(filename);
}

void Waypoints::read_data(std::string filename) {
    // Open input file
    ifstream *INPUT = new ifstream;
    INPUT->open(filename.c_str(), ifstream::in);
    if (!INPUT) {
        throw std::runtime_error("Can't open input file!");
    }
    /**
        Reading Instance
	**/
	std::cerr << "Begin reading Instance." << std::endl;
    //reading until D0
    char c;
    do {
        INPUT->get(c);
    } while ( c < '0' || c > '9');
    INPUT->putback(c);
    INPUT->putback('D'); /// Put back 'D' of depot name "D0"

    double xcoord, ycoord, demand, timewindow_begin, timewindow_end, service_time;
    std::string customer_id, node_type;

    /// CREATE ALL LOCI
    while (!INPUT->eof()) {
            (*INPUT) >> customer_id;
            (*INPUT) >> node_type;
            (*INPUT) >> xcoord;
            (*INPUT) >> ycoord;
            (*INPUT) >> demand;
            (*INPUT) >> timewindow_begin;
            (*INPUT) >> timewindow_end;
            (*INPUT) >> service_time;

            if (xcoord < 0) xcoord = -xcoord;
            if (ycoord < 0) ycoord = -ycoord;

            customers.push_back( std::make_pair( customer_id , std::make_pair( xcoord, ycoord) ) );

            /// FIND END OF TABLE
            (*INPUT) >> c;
            if ( c == 'Q' ) break; ///Referring to td_vrptw_instances
            INPUT->putback(c);
    }

    INPUT->close();
}

int Waypoints::get_time_horizon(std::string filename) {
    ifstream *INPUT = new ifstream;
    INPUT->open(filename.c_str(), ifstream::in);
    if (!INPUT) {
        throw std::runtime_error("Can't open input file!");
    }
    /**
        Reading Instance
    **/
    std::cerr << "Finding upper limit of Depot." << std::endl;
    //reading until D0
    char c;
    do {
        INPUT->get(c);
    } while ( c < '0' || c > '9');
    INPUT->putback(c);
    INPUT->putback('D'); /// Put back 'D' of depot name "D0"

    double xcoord, ycoord, demand, timewindow_begin, timewindow_end, service_time;
    std::string customer_id, node_type;

    (*INPUT) >> customer_id;
    (*INPUT) >> node_type;
    (*INPUT) >> xcoord;
    (*INPUT) >> ycoord;
    (*INPUT) >> demand;
    (*INPUT) >> timewindow_begin;
    (*INPUT) >> timewindow_end;
    (*INPUT) >> service_time;

    INPUT->close();
    return int(timewindow_end);
}
