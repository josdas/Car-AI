#pragma once
#include <vector>
#include "layers.h"
#include "active_layer.h"
#include <ostream>
#include "../my_stream.h"
#include "active_layer_const.h"


const size_t NUMBER_OF_ACTIVE_FUNCTION = 3;
enum layer_type {
	ACTIVE_A,
	ACTIVE_B,
	ACTIVE_L,
	ACTIVE_A_CONST,
	ACTIVE_B_CONST,
	ACTIVE_L_CONST,
};

struct Neural_coef {
	friend std::ostream& operator<<(std::ostream& out, const Neural_coef& a);
	std::vector<size_t> layers_size;
	std::vector<double> coefficient;
	std::vector<layer_type> layers_type;

	Neural_coef(const std::vector<size_t>& layers_size, 
				const std::vector<double>& coefficient, 
				const std::vector<layer_type>& layers_type);
};

inline std::ostream& operator<<(std::ostream& out, const Neural_coef& a) {
	out << a.layers_size.size() << '\n';
	for (auto v : a.layers_size) {
		out << v << ' ';
	}
	out << '\n';
	for (auto v : a.layers_type) {
		out << static_cast<int>(v) << ' ';
	}
	out << '\n';
	out << a.coefficient.size() << '\n';
	for (auto v : a.coefficient) {
		out << v << ' ';
	}
	return out;
}

class Neural_network {
	std::vector<std::shared_ptr<Layer> > layers;
public:
	Neural_network(const std::vector<Layer*>& layers);
	Neural_network(Neural_coef const& coef);

	std::vector<double> get(std::vector<double> const& data) const;
	std::vector<std::vector<std::vector<double> > > get_neurons_weight() const;
	Neural_coef get_coefficient() const;
};


inline Neural_coef::Neural_coef(const std::vector<size_t>& layers_size, 
								const std::vector<double>& coefficient, 
								const std::vector<layer_type>& layers_type):
	layers_size(layers_size),
	coefficient(coefficient),
	layers_type(layers_type) {}


inline Neural_network::Neural_network(const std::vector<Layer*>& _layers):
	layers(_layers.size()) {
	for (size_t i = 0; i < layers.size(); i++) {
		layers[i].reset(_layers[i]);
	}
}

inline Neural_network::Neural_network(Neural_coef const& coef) {
	My_stream stream(coef.coefficient);
	for (size_t i = 1; i < coef.layers_size.size(); i++) {
		switch(coef.layers_type[i - 1]) {
		case ACTIVE_A:
			layers.emplace_back(new Active_layer<active_function_A>(
				coef.layers_size[i - 1],
				coef.layers_size[i],
				stream
			));
			break;
		case ACTIVE_A_CONST:
			layers.emplace_back(new Actiev_layer_const<active_function_A>(
				coef.layers_size[i - 1],
				coef.layers_size[i],
				stream
			));
			break;
		case ACTIVE_B:
			layers.emplace_back(new Active_layer<active_function_B>(
				coef.layers_size[i - 1],
				coef.layers_size[i],
				stream
			));
			break;
		case ACTIVE_B_CONST:
			layers.emplace_back(new Actiev_layer_const<active_function_B>(
				coef.layers_size[i - 1],
				coef.layers_size[i],
				stream
			));
			break;
		case ACTIVE_L:
			layers.emplace_back(new Active_layer<active_function_linear>(
				coef.layers_size[i - 1],
				coef.layers_size[i],
				stream
			));
			break;
		case ACTIVE_L_CONST:
			layers.emplace_back(new Actiev_layer_const<active_function_linear>(
				coef.layers_size[i - 1],
				coef.layers_size[i],
				stream
			));
			break;
		default: 
			assert(false);
		}
	}
	assert(stream.end());
}

inline std::vector<double> Neural_network::get(std::vector<double> const& data) const {
	std::vector<double> result = data;
	for (size_t i = 0; i < layers.size(); i++) {
		result = layers[i]->get(result);
	}
	return result;
}

inline std::vector<std::vector<std::vector<double> > > Neural_network::get_neurons_weight() const {
	std::vector<std::vector<std::vector<double> > > result(layers.size());
	for (size_t i = 0; i < layers.size(); i++) {
		result[i] = layers[i]->get_neurons_weight();
	}
	return result;
}

inline Neural_coef Neural_network::get_coefficient() const {
	std::vector<double> result;
	for (size_t i = 0; i < layers.size(); i++) {
		auto temp = layers[i]->get_coefficient();
		for (auto v : temp) {
			result.push_back(v);
		}
	}
	std::vector<size_t> layers_size;
	std::vector<layer_type> layers_type(layers.size());
	for (size_t i = 0; i < layers.size(); i++) {
		layers_type[i] = static_cast<layer_type>(layers[i]->get_type());
	}

	layers_size.push_back(layers.front()->get_input_size());
	for (auto const& v : layers) {
		layers_size.push_back(v->get_output_size());
	}
	return Neural_coef(layers_size, result, layers_type);
}
