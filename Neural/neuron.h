#pragma once
#include <algorithm>

const size_t NUMBER_OF_ACTIVE_FUNCTION = 3;
enum layer_type {
	ACTIVE_A,
	ACTIVE_B,
	ACTIVE_L,
	ACTIVE_A_CONST,
	ACTIVE_B_CONST,
	ACTIVE_L_CONST,
	MAX
};

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