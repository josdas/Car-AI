#include <iostream>
#include <memory>
#include "Neural/neural_network.h"
#include <ctime>
#include <fstream>
#include "my_random.h"
#include "race_log.h"
#include "race.h"
#include <string>
using namespace std;

void print_coeff(Neural_coef const& a, string name = "Result") {
	name += ".txt";
	ofstream out(name.c_str(), ofstream::out);
	out << a << '\n';
	out.close();
}

void print_log(Race_log const& a, string name = "Log") {
	name += ".txt";
	ofstream out(name.c_str(), ofstream::out);
	out << D_TIME << '\n';
	out << a.coin << ' ' << a.time << '\n';
	out << a.points.size() << '\n';
	for (size_t i = 0; i < a.points.size(); i++) {
		auto point = a.points[i];
		out << point.x << ' ' << point.y << ' ' << a.direction[i] << '\n';
	}
	out << a.goals.size() << '\n';
	for (auto point : a.goals) {
		out << point.x << ' ' << point.y << '\n';
	}
	out.close();
}

Neural_coef read_coeff(string name) {
	name = "Result_" + name + ".txt";
	ifstream in(name.c_str(), ifstream::in);
	int n, m;
	in >> n;
	vector<size_t> layers_size(n);
	for (auto& v : layers_size) {
		in >> v;
	}
	vector<layer_type> layers_type(n - 1);
	for (auto& v : layers_type) {
		int t;
		in >> t;
		v = static_cast<layer_type>(t);
	}
	in >> m;
	vector<double> coefficient(m);
	for (auto& v : coefficient) {
		in >> v;
	}
	return Neural_coef(layers_size, coefficient, layers_type);
}

double get_time() {
	return clock() * 1.0 / CLOCKS_PER_SEC;
}

Race generate_race(int goal_count, double size) {
	vector<Point> goals(goal_count);
	for(auto &v : goals) {
		v = get_rand_point(size);
	}
	Point start_pos = get_rand_point(size);
	Point start_dir = get_rand_point(size);
	if(is_zero(length(start_dir))) {
		start_dir.x += 0.1;
	}
	start_dir = normolize(start_dir);
	return Race(goals, start_pos, start_dir);
}

Race_log run_race(Race const& race, Car& car) {
	return race.play(&car);
}

double test(Neural_network const& neural_network, vector<Race> const& races, double s = 1000000) {
	double sum = 0;
	double mi = INF;
	Car car(neural_network);
	for(auto const& race : races) {
		auto log = run_race(race, car);
		double t = (race.goal_count() - log.coin) * s + log.time + log.value / 3000 * 0;
		sum += t;
		mi = max(t, mi);
	}
	return -(sum + mi) / 2;
}
double super_test(Neural_network const& neural_network, vector<Race> const& races, double s = -10) {
	double sum = 0;
	Car car(neural_network);
	for (auto const& race : races) {
		auto log = run_race(race, car);
		sum += log.coin;
	}
	return sum;
}

int main() {
	auto neural_network = Neural_network(read_coeff("n1"));

//	auto neural_network = Neural_network(vector<Layer*>{
//		new Actiev_layer_const<active_function_B>(13, 25),
//		new Actiev_layer_const<active_function_B>(25, 25),
//		new Actiev_layer_const<active_function_B>(25, 2)
//	});
	vector<Race> races;
	double result = 0;
	double s = 3, d = 10;
	for (int i = 0; get_time() < TIME_TO_END; i++) {
		if(i % 30 == 0) {
			races.clear();
			for (int i = 0; i < RACE_COUNT; i++) {
				races.push_back(generate_race(
					rand() % MAX_COINT + 1,
					MAX_SIZE / 4 + get_rand_udouble(MAX_SIZE))
				);
			}
			result = test(neural_network, races);
		}

		auto coef = neural_network.get_coefficient();

		double new_res = result;
		auto test_net = neural_network;

		for (int it = 0; it < 10; it++) {
			int number_it = static_cast<int>(get_rand_udouble(d) + 1);
			vector<pair<int, double> > vector_temp;
			auto temo_coeff = coef;
			double sum = 0;
			for (int j = 0; j < number_it; j++) {
				int x = get_rand_int(static_cast<int>(coef.coefficient.size()));
				double delata = get_rand_double(s);
				vector_temp.push_back({ x, delata });
			}
			for (auto &v : vector_temp) {
				temo_coeff.coefficient[v.first] += v.second;
			}
			auto temp_net = Neural_network(temo_coeff);
			double temp_res = test(temp_net, races);
			if (temp_res > new_res) {
				new_res = temp_res;
				test_net = temp_net;
			}
		}

		if (new_res > result) {
			result = new_res;
			neural_network = test_net;
			cerr << "New result=" << new_res << ' '
				<< "super=" << super_test(neural_network, races) << ' '
				<< "i=" << i << ' '
				<< "Time=" << get_time() << '\n'
				<< "S=" << s << ' '
				<< "D=" << d << "\n\n";
		}
		if(i % 50 == 0) {
			d *= 0.98;
			s *= 0.99;
		}
	}

	print_coeff(neural_network.get_coefficient());
	{
		Car car(neural_network);
		for (int i = 0; i < 50; i++) {
			Race race = generate_race(
				rand() % MAX_COINT + 1,
				MAX_SIZE / 4 + get_rand_udouble(MAX_SIZE)
			);
			auto log = race.play(&car);
			print_log(log, "Log_" + to_string(i));
		}
	}
}
