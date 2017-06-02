#pragma once
#include "point.h"
#include <vector>

struct Race_log {
	enum DIRECTION_TYPE {
		FORWARD,
		BACK
	};
	std::vector<Point> points;
	std::vector<Point> goals;
	std::vector<DIRECTION_TYPE> direction;
	double time;
	double value;
	int coin;
	Race_log();
};

inline Race_log::Race_log(): time(0), value(0), coin(0) {}
