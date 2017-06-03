#pragma once
#include "neuron.h"
#include <vector>
#include "layers.h"
#include <cassert>

class Max_layer : public Layer {
public:
	explicit Max_layer(const std::vector<std::vector<double> >& neurons);
	Max_layer(size_t in, size_t out);
	Max_layer(size_t in, size_t out, My_stream& stream);
	~Max_layer() override = default;

	virtual std::vector<double> get(std::vector<double> const& data) const override;
	int get_type() override;
};

inline Max_layer::Max_layer(const std::vector<std::vector<double> >& neurons) : 
	Layer(neurons) {}

inline Max_layer::Max_layer(size_t in, size_t out) : Layer(in, out) {}

inline Max_layer::Max_layer(size_t in, size_t out, My_stream& stream) :
	Layer(in, out, stream) {}

inline std::vector<double> Max_layer::get(std::vector<double> const& data) const {
	assert(input_size == data.size());
	std::vector<std::pair<double, size_t> > temp(output_size);
	for (size_t i = 0; i < output_size; i++) {
		double sum = 0;
		for (int j = 0; j < input_size; j++) {
			sum += data[j] * neurons[i][j];
		}
		temp[i] = {sum, i};
	}
	sort(temp.begin(), temp.end());
	std::vector<double> result(output_size);
	for(int i = 0; i < 2; i++) {
		result[temp[i].second] = temp[i].first;
	}
	return result;
}

inline int Max_layer::get_type() {
	return (int)layer_type::MAX;
}