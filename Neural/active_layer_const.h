#pragma once
#include "neuron.h"
#include <vector>
#include "layers.h"
#include "active_layer.h"

template<class T>
class Actiev_layer_const: public Active_layer<T> {
	std::vector<double> cnst;
	std::vector<double> change(std::vector<double>& data) const;
public:
	Actiev_layer_const(const std::vector<std::vector<double> >& neurons, const std::vector<double>& cnst);
	Actiev_layer_const(size_t in, size_t out);
	Actiev_layer_const(size_t in, size_t out, My_stream& stream);
	~Actiev_layer_const() override = default;

	std::vector<double> get(std::vector<double> const& data) const override;
	std::vector<double> get_coefficient() const override;
	int get_type() override;
};

template <class T>
Actiev_layer_const<T>::Actiev_layer_const(const std::vector<std::vector<double> >& neurons, const std::vector<double>& cnst): 
	Active_layer<T>(neurons),
	cnst(cnst) {}

template <class T>
Actiev_layer_const<T>::Actiev_layer_const(size_t in, size_t out): 
	Active_layer<T>(in, out),
	cnst(out) {
	for(auto &v : cnst) {
		v = get_rand_double(0.5);
	}
}

template <class T>
Actiev_layer_const<T>::Actiev_layer_const(size_t in, size_t out, My_stream& stream):
	Active_layer<T>(in, out, stream),
	cnst(stream.get_block(out)) {}

template <class T>
std::vector<double> Actiev_layer_const<T>::get(std::vector<double> const& data) const {
	return active(change(get_sum(data)));
}

template <class T>
std::vector<double> Actiev_layer_const<T>::get_coefficient() const {
	std::vector<double> result;
	for (size_t i = 0; i < output_size; i++) {
		for (auto v : neurons[i]) {
			result.push_back(v);
		}
	}
	for(auto v : cnst) {
		result.push_back(v);
	}
	return result;
}

template <class T>
int Actiev_layer_const<T>::get_type() {
	return T::type_number + NUMBER_OF_ACTIVE_FUNCTION;
}

template <class T>
std::vector<double> Actiev_layer_const<T>::change(std::vector<double>& data) const {
	for(size_t i = 0; i < data.size(); i++) {
		data[i] += cnst[i];
	}
	return data;
}
