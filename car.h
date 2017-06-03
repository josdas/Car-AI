#pragma once
#include <vector>
#include "point.h"
#include <algorithm>
#include "Neural/neural_network.h"

const int HALF_SECTOR_COUNT = 6;
const double NEURAL_INF = 1e3;

class Car {
	Neural_network neural_network;
public:
	explicit Car(const Neural_network& neural_network);
	virtual ~Car() = default;

	virtual std::pair<double, double> action(Point pos, Point dir, double v, std::vector<Point> const& goals);
};

inline Car::Car(const Neural_network& neural_network): neural_network(neural_network) {}

inline std::pair<double, double> Car::action(Point pos, Point dir, double v, std::vector<Point> const& goals) {
	std::vector<double> data(HALF_SECTOR_COUNT * 4 + 1, NEURAL_INF);

	for (auto goal : goals) {
		double e = atan2(cross_product(goal - pos, dir), dot_product(goal - pos, dir));
		for (double sector = -HALF_SECTOR_COUNT; sector < HALF_SECTOR_COUNT; sector++) {
			double l = PI * sector / HALF_SECTOR_COUNT;
			double r = l + PI / HALF_SECTOR_COUNT;
			if (l <= e && e < r) {
				int ind = sector + HALF_SECTOR_COUNT;
				data[ind] = std::min(data[ind], distance(pos, goal));
			}

			e += PI / HALF_SECTOR_COUNT / 2;
			if(e > PI) {
				e -= 2 * PI;
			}

			if (l <= e && e < r) {
				int ind = sector + HALF_SECTOR_COUNT * 2 + 1;
				data[ind] = std::min(data[ind], distance(pos, goal));
			}
		}
	}
	data[HALF_SECTOR_COUNT * 2] = v;

	auto result = neural_network.get(data);
	assert(!isnan(result[0]));
	assert(!isnan(result[1]));
	if(isnan(result[0])) {
		std::cerr << "FUCK";
		exit(1);
	}
	return { result[0], result[1] };
}
