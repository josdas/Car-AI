#pragma once
#include <algorithm>

struct active_function_A {
	static double active_function(double x) {
		if (x > 30)
			x = 30;
		if (x < -30)
			x = -30;
		return 1 / (1 + std::exp(-x));
	}
	static const int type_number = 0;
};

struct active_function_B {
	static double active_function(double x) {
		if (x > 30)
			x = 30;
		if (x < -30)
			x = -30;
		return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
	}
	static const int type_number = 1;
};

struct active_function_linear {
	static double active_function(double x) {
		return std::max(x, 0.0);
	}
	static const int type_number = 2;
};