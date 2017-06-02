#pragma once
#include <cmath>

const double EPS = 1e-5;
const double PI = -4 * atan(-1.0);
const double INF = 1e9;

inline bool is_zero(double x) {
	return abs(x) < EPS;
}
struct Point {
	union {
		struct {
			double x, y;
		};
		double data[2];
	};

	Point(double _x, double _y);
	Point();
	Point(Point const& b) : Point(b.x, b.y) { }

	friend Point operator+(Point a, Point b);
	friend Point operator-(Point a, Point b);

	double& operator[](size_t t);
	double operator[](size_t t) const;

	friend bool operator<(Point a, Point b);
	friend bool operator==(Point a, Point b);
	friend bool operator<=(Point a, Point b);

	Point operator/(double c) const;
	Point operator*(double c) const;
	Point operator-=(Point b);
};

inline Point::Point(double _x, double _y) : x(_x), y(_y) { }

inline Point::Point() : Point(0, 0) { }

inline double& Point::operator[](size_t t) {
	return data[t];
}

inline double Point::operator[](size_t t) const {
	return data[t];
}

inline Point Point::operator-=(Point b) {
	return *this = *this - b;
}

inline Point Point::operator/(double c) const {
	return Point(x / c, y / c);
}

inline Point Point::operator*(double c) const {
	return Point(x * c, y * c);
}

inline double cross_product(Point a, Point b) {
	return a.x * b.y - a.y * b.x;
}

inline double dot_product(Point a, Point b) {
	return a.x * b.x + a.y * b.y;
}

inline Point operator+(Point a, Point b) {
	return Point(a.x + b.x, a.y + b.y);
}

inline Point operator-(Point a, Point b) {
	return Point(a.x - b.x, a.y - b.y);
}

inline bool operator<(Point a, Point b) {
	for (int i = 0; i < 2; i++) {
		if (!is_zero(a[i] - b[i])) {
			return a[i] < b[i];
		}
	}
	return false;
}

inline bool operator==(Point a, Point b) {
	return is_zero(a.x - b.x) && is_zero(a.y - b.y);
}

inline bool operator<=(Point a, Point b) {
	return a < b || a == b;
}

inline double length(Point s) {
	return sqrt(s.x * s.x + s.y * s.y);
}

inline double distance(Point a, Point b) {
	return length(a - b);
}

inline Point rotation(Point a, double angle) {
	return Point(
		a.x * cos(angle) - a.y * sin(angle),
		a.x * sin(angle) + a.y * cos(angle)
	);
}

inline Point normolize(Point a) {
	return a / length(a);
}