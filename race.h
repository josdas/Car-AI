#pragma once
#include <vector>
#include "point.h"
#include "car.h"
#include "race_log.h"
#include "my_const.h"


class Race {
	std::vector<Point> start_goals;
	Point start_position;
	Point start_direction;

public:
	size_t goal_count() const;
	Race(const std::vector<Point>& start_goals, const Point& start_position, const Point& start_direction);
	Race_log play(Car* car) const;
};

inline size_t Race::goal_count() const {
	return start_goals.size();
}

inline Race::Race(const std::vector<Point>& start_goals, const Point& start_position, const Point& start_direction): start_goals(start_goals),
                                                                                                                     start_position(start_position),
                                                                                                                     start_direction(start_direction) {}

inline Race_log Race::play(Car* car) const {
	Race_log log;
	log.goals = start_goals;
	auto pos = start_position;
	auto dir = start_direction;
	auto goals = start_goals;
	double v = 0;

	for (double time = 0; time < MAX_TIME; time += D_TIME) {
		{
			std::vector<Point> new_goals;
			for (auto goal : goals) {
				if (distance(pos, goal) > GOAL_RANGE) {
					new_goals.push_back(goal);
					log.value += distance(pos, goal);
				}
				else {
					log.coin++;
				}
			}
			goals = new_goals;
		}
		if (goals.empty()) {
			break;
		}

		log.time = time;
		log.points.push_back(pos);
		auto action = car->action(pos, dir, v, goals);
		double speed = action.first;
		double angle = action.second;
		if (speed > 0) {
			v += speed * MAX_F_SPEED * D_TIME;
			log.direction.push_back(Race_log::FORWARD);
		}
		else {
			v += speed * MAX_B_SPEED * D_TIME;
			log.direction.push_back(Race_log::BACK);
		}
		if(v > MAX_SPEED) {
			v = MAX_SPEED;
		}
		if (v < -MAX_SPEED) {
			v = -MAX_SPEED;
		}
		dir = normolize(rotation(dir, angle * D_TIME * ROTATION_SPEED * v / MAX_SPEED)); // TODO

		pos = pos + dir * v * D_TIME;
	}
	return log;
}
