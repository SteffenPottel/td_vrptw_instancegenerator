//---------------------------------------------------------------------------
#include <cmath>
#include <stdexcept>
//---------------------------------------------------------------------------

double round3(double num) {
	double result = std::round(num * 1000) / 1000;
	return result;
}

long int get_slope( int x, double fx, int y, double fy) {
	long int result = 0;
	double eps = 0;
	int preci = 1000;
	if ( (fx - fy)*(fx - fy) > eps && x != y ) {
		result = int( preci * (fy-fx)/(y-x) );
    }
	else if (x == y) {
		throw std::invalid_argument("Division by zero!");
	}
	return result;
}

long int get_intercept(int x, long int fx, long int slope) {
	long int result = fx - slope * x ;
	return result;
}


