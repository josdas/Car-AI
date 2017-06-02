#pragma once
#include <cstdlib>
#include "point.h"

inline double get_rand_double(double s) {
	return 2.0 * (rand() - RAND_MAX / 2) / RAND_MAX * s;
}

inline double get_rand_udouble(double s) {
	return 1.0 * rand() / RAND_MAX * s;
}

inline Point get_rand_point(double s) {
	return Point(get_rand_udouble(s), get_rand_udouble(s));
}

inline int get_rand_int(int x) {
	return rand() % x;
}
