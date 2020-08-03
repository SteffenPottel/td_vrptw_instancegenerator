//---------------------------------------------------------------------------
#ifndef TOOLSH
#define TOOLSH

#include <vector>
//---------------------------------------------------------------------------

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

double round3(double num);

long int get_slope( int x, double fx, int y, double fy);

long int get_intercept(int x, long int fx, long int slope);

#endif
