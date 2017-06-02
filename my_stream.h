#pragma once
#include <cstdlib>
#include <vector>

class My_stream {
	std::vector<double> data;
	size_t ind;
public:
	My_stream(const std::vector<double>& data);

	bool end() const;

	double next();
	std::vector<double> get_block(size_t n);
	std::vector<std::vector<double> > get_blocks(size_t n, size_t m);
};

inline My_stream::My_stream(const std::vector<double>& data): 
	data(data),
	ind(0) {}

inline bool My_stream::end() const {
	return ind == data.size();
}

inline double My_stream::next() {
	if(end()) {
		throw std::runtime_error("Stream is end");
	}
	return data[ind++];
}

inline std::vector<double> My_stream::get_block(size_t n) {
	std::vector<double> res(n);
	for (size_t i = 0; i < n; i++) {
		res[i] = next();
	}
	return res;
}

inline std::vector<std::vector<double> > My_stream::get_blocks(size_t n, size_t m) {
	std::vector<std::vector<double> > res(n);
	for (size_t i = 0; i < n; i++) {
		res[i] = get_block(m);
	}
	return res;
}
