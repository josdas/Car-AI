#pragma once
#include "neuron.h"
#include <vector>
#include "layers.h"
#include <cassert>

template<class T>
class Active_layer: public Layer {
protected:
	std::vector<double> active(std::vector<double>& data) const;
	std::vector<double> get_sum(std::vector<double> const& data) const;

	virtual void resize_other(int in, int out) {};
public:
	explicit Active_layer(const std::vector<std::vector<double> >& neurons);
	Active_layer(size_t in, size_t out);
	Active_layer(size_t in, size_t out, My_stream& stream);
	~Active_layer() override = default;

	virtual std::vector<double> get(std::vector<double> const& data) const override;
	int get_type() override;

	virtual void resize(int in, int out) override;
};

template <class T>
std::vector<double> Active_layer<T>::active(std::vector<double>& data) const {
	for(auto &v : data) {
		v = T::active_function(v);
	}
	return data;
}

template <class T>
std::vector<double> Active_layer<T>::get_sum(std::vector<double> const& data) const {
	assert(input_size == data.size());
	std::vector<double> result(output_size);
	for (size_t i = 0; i < output_size; i++) {
		double sum = 0;
		for (int j = 0; j < input_size; j++) {
			sum += data[j] * neurons[i][j];
		}
		result[i] = sum;
	}
	return result;
}

template <class T>
Active_layer<T>::Active_layer(const std::vector<std::vector<double> >& neurons): Layer(neurons) {}

template <class T>
Active_layer<T>::Active_layer(size_t in, size_t out): Layer(in, out) {}

template <class T>
Active_layer<T>::Active_layer(size_t in, size_t out, My_stream& stream):
	Layer(in, out, stream) {}

template <class T>
std::vector<double> Active_layer<T>::get(std::vector<double> const& data) const {
	return active(get_sum(data));
}

template <class T>
int Active_layer<T>::get_type() {
	return T::type_number;
}

template <class T>
void Active_layer<T>::resize(int in, int out) {
	resize_other(in, out);
	neurons.resize(out);
	for (size_t i = 0; i < out; i++) {
		neurons[i].resize(in);
		for (size_t j = input_size; j < in; j++) {
			neurons[i][j] = get_rand_double(0.5);
		}
	}
	input_size = in;
	output_size = out;
}
