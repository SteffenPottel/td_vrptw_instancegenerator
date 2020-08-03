//---------------------------------------------------------------------------
#ifndef PWLH
#define PWLH

#include "Segment.h"

#include <vector>
//---------------------------------------------------------------------------
namespace InstanceGenerator{

	/**
		The PWL class captures all tools and properties of
		piecewise-linear functions.
	**/
	class PWL {
	public:
		std::vector<Segment> segments;
		std::vector<FloatSegment> float_segments;
	public:
		PWL(int start, int ending, long int slope, long int intercept);
		PWL(double scale);
		PWL(std::vector<std::pair<int, double>>& value_pairs, int segments_num);
		PWL() {};
		~PWL() {};

		double get_value(double argument);
		void generate_segments(std::vector<std::pair<int, double>> value_pairs);
		bool continuity();
		bool fifo();
		bool fifo_with_corrector();
		bool domain();
		bool order();
		void valid_travel();
		void valid_consumption();
		void show();
	};
}
#endif
