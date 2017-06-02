#pragma once
#include "neuron.h"
#include "../my_random.h"
#include "../my_stream.h"
#include "neural_network.h"

class My_stream;

class Layer {
protected:
	std::vector<std::vector<double> > neurons;
	size_t input_size;
	size_t output_size;

public:
	virtual ~Layer() = default;
	explicit Layer(const std::vector<std::vector<double> >& neurons);
	explicit Layer(size_t in, size_t out);
	explicit Layer(size_t in, size_t out, My_stream& stream);

	virtual std::vector<double> get(std::vector<double> const& data) const = 0;

	std::vector<std::vector<double> > const& get_neurons_weight() const;
	virtual std::vector<double> get_coefficient() const;

	void set(size_t a, size_t b, double x);

	size_t get_input_size() const;
	size_t get_output_size() const;
	virtual int get_type() = 0;
};

inline Layer::Layer(const std::vector<std::vector<double> >& neurons):
	neurons(neurons), 
	output_size(neurons.size()) {
	if(neurons.size() == 0) {
		input_size = 0;
	}
	else {
		input_size = neurons[0].size();
	}
}

inline Layer::Layer(size_t in, size_t out):
	neurons(out),
	input_size(in),
	output_size(out) {
	for (size_t i = 0; i < out; i++) {
		neurons[i].resize(in);
		for (size_t j = 0; j < in; j++) {
			neurons[i][j] = get_rand_double(0.5);
		}
	}
}

inline Layer::Layer(size_t in, size_t out, My_stream& stream) :
	neurons(stream.get_blocks(out, in)),
	input_size(in),
	output_size(out) {}

inline std::vector<std::vector<double> > const& Layer::get_neurons_weight() const {
	return neurons;
}

inline std::vector<double> Layer::get_coefficient() const {
	std::vector<double> result;
	for (size_t i = 0; i < output_size; i++) {
		for (auto v : neurons[i]) {
			result.push_back(v);
		}
	}
	return result;
}

inline void Layer::set(size_t a, size_t b, double x) {
	neurons[a][b] = x;
}

inline size_t Layer::get_input_size() const {
	return input_size;
}

inline size_t Layer::get_output_size() const {
	return output_size;
}