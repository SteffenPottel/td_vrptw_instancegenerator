//---------------------------------------------------------------------------
#include "Segment.h"

#include <stdexcept>
//---------------------------------------------------------------------------
using namespace InstanceGenerator;

bool Segment::in_interval(double argument) {
	bool in = false;

	if (start <= argument && ending >= argument) in = true;

	return in;
}

double Segment::get_seg_value(double argument) {
	double result;

	if (this->in_interval(argument)) {
		result = slope * argument + intercept;
	}
	else {
		throw std::invalid_argument("Passed argument is not in the interval");
	}

	return result;
}

bool FloatSegment::in_interval(double argument) {
	bool in = false;

	if (start <= argument && ending >= argument) in = true;

	return in;
}

double FloatSegment::get_seg_value(double argument) {
	double result;

	if (this->in_interval(argument)) {
		result = slope * argument + intercept;
	}
	else {
		throw std::invalid_argument("Passed argument is not in the interval");
	}

	return result;
}
