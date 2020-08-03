//---------------------------------------------------------------------------
#include "PWL.h"

#include "Segment.h"
#include "Tools.h"

#include <vector>
#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdexcept>
//---------------------------------------------------------------------------
using namespace InstanceGenerator;

PWL::PWL(int start, int ending, long int slope, long int intercept) {
	segments.push_back(Segment(start, ending, slope, intercept) );
}

PWL::PWL(std::vector<std::pair<int, double>>& value_pairs, int segments_num) {
	if (value_pairs.size() > segments_num + 1) {
		throw std::logic_error("Constructor would generate too many segments.");
	}
	this->generate_segments(value_pairs);
}

PWL::PWL(double scale) {
	///Purely constructed for congestion timing!
	int breakingpoints[10] = {int(0*scale), int(190*scale), int(210*scale), int(390*scale), int(410*scale), int(590*scale), int(610*scale), int(790*scale), int(810*scale), int(1000*scale)};
	int values_at_bp[10] = {0, 0, 1, 1, 0, 0, 1, 1, 0, 0};
	double slope, intercept;
	double corrector = 1; ///For the short instances and steep gradients - correcting the slope
	if (scale < 0.5) corrector = 0.5;

	for (int i=0; i < 9; i++) {
		slope = corrector * ((float)values_at_bp[i+1] - (float)values_at_bp[i] ) / ((float)breakingpoints[i+1] - (float)breakingpoints[i] );
		intercept = corrector * (float)values_at_bp[i] - slope * (float)breakingpoints[i];
		float_segments.push_back(FloatSegment(breakingpoints[i], breakingpoints[i+1], slope, intercept));
	}
}

double PWL::get_value(double argument) {
	double result;
	for (std::vector<Segment>::iterator it = segments.begin(); it != segments.end(); ++it) {
		if (it->in_interval(argument) ) {
			result = it->get_seg_value(argument);
			break;
		}
    }
    for (std::vector<FloatSegment>::iterator it = float_segments.begin(); it != float_segments.end(); ++it) {
		result = 0;
		if (it->in_interval(argument) ) {
			result = it->get_seg_value(argument);
			break;
		}

	}

	return result;
}

bool PWL::continuity() {
	bool continuous = true;
	long int left_function_value = -1;
	long int right_function_value = segments.begin()->start * segments.begin()->slope + segments.begin()->intercept ;

	for (std::vector<Segment>::const_iterator it = segments.begin(); it != segments.end(); ++it) {
		left_function_value =  it->start * it->slope + it->intercept;
		if ( left_function_value != right_function_value ) {
			continuous = false;
			break;
		}
		right_function_value = it->ending * it->slope + it->intercept ;
	}

	return continuous;
}

bool PWL::fifo() {
	bool fifo = true;

	for (std::vector<Segment>::const_iterator it = segments.begin(); it != segments.end(); ++it) {
		if (it->slope < -1000) {
			fifo = false;
			break;
		}
	}

	return fifo;
}

bool PWL::fifo_with_corrector() {
	bool fifo = false;

	return fifo;
}

bool PWL::domain() {
	bool connected = true;
	int left_value, right_value;

	right_value = segments.begin()->start;
	for (std::vector<Segment>::const_iterator it = segments.begin(); it != segments.end(); ++it) {
		left_value = it->start;
		if (left_value != right_value) {
			connected = false;
			break;
		}
		right_value = it->ending;
	}

	return connected;
}

bool PWL::order() {
	bool ordered = true;

	for (std::vector<Segment>::const_iterator it = segments.begin(); it != segments.end(); ++it) {
		if (it->start >= it->ending) {
			ordered = false;
			break;
		}
	}

	return ordered;
}

void PWL::valid_travel() {
	if (!this->continuity() ) {
		throw std::invalid_argument("Function is not continuous.");
	}
	if (!this->fifo() ) {
		throw std::invalid_argument("Function violates FIFO assumption.");
	}
	if (!this->domain() ) {
		throw std::invalid_argument("Domain is not continuous.");
	}
	if (!this->order() ) {
		throw std::invalid_argument("Segment has malacious domain.");
	}
}

void PWL::valid_consumption() {
	if (!this->continuity() ) {
		throw std::invalid_argument("Function is not continuous.");
	}
	if (!this->domain() ) {
		throw std::invalid_argument("Domain is not continuous.");
	}
	if (!this->order() ) {
		throw std::invalid_argument("Segment has malacious domain.");
	}
}

void PWL::show() {
    std::cout << "Start\t" << "End\t" << "Slope\t" << "Intercept" << std::endl;
    for (std::vector< Segment >::iterator it = segments.begin(); it != segments.end(); ++it ) {
        std::cout << it->start << "\t" << it->ending << "\t" << it->slope << "\t" << it->intercept << std::endl;
    }
}

void PWL::generate_segments(std::vector<std::pair<int, double>> value_pairs) {
	long int temp_slope, temp_intercept;
	long int left_function_value;
	temp_slope = get_slope(value_pairs[0].first, value_pairs[0].second, value_pairs[1].first, value_pairs[1].second);
	temp_intercept = int(value_pairs[0].second * 1000) - temp_slope * value_pairs[0].first;;
	segments.push_back( Segment( value_pairs[0].first, value_pairs[1].first, temp_slope, temp_intercept ) );

	for (unsigned int l=1; l < value_pairs.size()-1; l++) {
		left_function_value = segments.back().ending * segments.back().slope + segments.back().intercept;
		temp_slope = get_slope(value_pairs[l].first, value_pairs[l].second, value_pairs[l+1].first, value_pairs[l+1].second);
		temp_intercept = get_intercept(value_pairs[l].first, left_function_value, temp_slope);
		segments.push_back( Segment(value_pairs[l].first, value_pairs[l+1].first, temp_slope, temp_intercept) );
	}
}
