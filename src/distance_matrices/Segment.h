//---------------------------------------------------------------------------
#ifndef SEGMENTH
#define SEGMENTH

//---------------------------------------------------------------------------
namespace InstanceGenerator{

	/**
		The Segment class is the base for pwl-functions.
	**/
	class Segment {
	public:
		int start, ending;
		long int slope, intercept;
	public:
		Segment(int st, int en, long int sl, long int in) : start(st), ending(en), slope(sl), intercept(in) {};
		~Segment() {};

		bool in_interval(double argument);
		double get_seg_value(double argument);
	};

	class FloatSegment {
	public:
		int start, ending;
		double slope, intercept;
	public:
		FloatSegment(int st, int en, double sl, double in) : start(st), ending(en), slope(sl), intercept(in) {};
		~FloatSegment() {};

		bool in_interval(double argument);
		double get_seg_value(double argument);
	};
}
#endif
