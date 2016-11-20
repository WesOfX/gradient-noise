#include "gradient_noise.hpp"
#include <array>

template<typename float_type, std::size_t dimension_count>
gnd::gradient_noise<float_type, dimension_count>::gradient_noise(seed_type seed): m_seed(seed){}

template<typename float_type, std::size_t dimension_count>
void gnd::gradient_noise<float_type, dimension_count>::seed(seed_type val){m_seed = val;}

template<typename float_type, std::size_t dimension_count>
float_type gnd::gradient_noise<float_type, dimension_count>::operator()(std::array<float_type, dimension_count> position){
	std::array<seed_type, dimension_count> unit_position; // The unit position
	std::array<float_type, dimension_count> offset_position = position; // The unit position offset
	for(std::size_t dimension = 0; dimension < dimension_count; dimension++){
		unit_position[dimension] = (int)floor(position[dimension]);
		offset_position[dimension] -= unit_position[dimension];
	}
	std::array<float_type, (std::size_t)pow(4, (double)dimension_count)> nodes; // The psuedorandom nodes to be interpolated
	for(std::size_t node = 0; node < nodes.size(); node++){
		std::array<seed_type, dimension_count> node_position;
		for(std::size_t dimension = 0; dimension < dimension_count; dimension++){
			node_position[dimension] = (node / (int)pow(4, (double)dimension)) % 4 + unit_position[dimension];
		}
		std::seed_seq seq(node_position.begin(), node_position.end()); // Make a seed sequence from the node position
		std::array<seed_type, 1> node_seed;
		seq.generate(node_seed.begin(), node_seed.end()); // Get the node seed
		node_seed[0] |= m_seed; // Apply base seed
		engine_type engine(node_seed[0]);
		engine.discard(minimum_discard_count); // Escape from zero-land
		nodes[node] = distribution_type(-1.0, 1.0)(engine); // Get node value
	}
	for(std::size_t dimension = 0; dimension < dimension_count; dimension++){
		const auto interpolated_node_count = (nodes.size() / 4) / (int)pow(4, (double)dimension);
		for(std::size_t interpolated_node = 0; interpolated_node < interpolated_node_count; interpolated_node++){
			auto node = interpolated_node * 4;
			nodes[interpolated_node] = cerp(nodes[node], nodes[node + 1], nodes[node + 2], nodes[node + 3],	offset_position[dimension]);
		}
	}
	return nodes[0]; // The final noise value
}

template<typename float_type, std::size_t dimension_count>
float_type gnd::gradient_noise<float_type, dimension_count>::cerp(float_type y0, float_type y1, float_type y2, float_type y3, float_type mu){
	float_type a0 = y3 - y2 - y0 + y1, a1 = y0 - y1 - a0, a2 = y2 - y0, a3 = y1, mu2 = mu * mu;
	return a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3;
}
