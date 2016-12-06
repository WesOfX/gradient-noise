#pragma once
#include <random>
#include <array>

// "Generic N-Dimensional"
namespace gnd{
/* float_type is the type of the noise values and the noise parameters
   dimension_count is the number of dimensions the noise has */
template<typename float_type, std::size_t dimension_count>
class gradient_noise{
public:
	// The type of the engine
	typedef std::default_random_engine engine_type;

	// The type of the distribution
	typedef std::uniform_real_distribution<float_type> distribution_type;

	// The type of the seed of the engine and the noise
	typedef engine_type::result_type seed_type;

	// The number of random numbers discarded after a new engine seed
	static constexpr unsigned int discard_count = 1;

	// Construct gradient noise with an optional seed
	gradient_noise(engine_type::result_type seed = engine_type::default_seed): m_seed(seed){}

	// Seed the gradient noise
	void seed(seed_type val = engine_type::default_seed){m_seed = val;}

	// Return a noise value from an n-dimensional position
	auto operator()(std::array<float_type, dimension_count> position){
		// The unit position
		std::array<seed_type, dimension_count> unit_position;

		// The unit position offset
		std::array<float_type, dimension_count> offset_position = position;

		// For every dimension
		for(std::size_t dimension = 0; dimension < dimension_count; dimension++){
			// The unit position is the same as the position, but it's the integer floor of all axis positions
			unit_position[dimension] = (int)floor(position[dimension]);

			// The offset position is the position within the node 'cell'
			offset_position[dimension] -= unit_position[dimension];
		}

		// The psuedorandom nodes to be interpolated
		std::array<float_type, (std::size_t)pow(4, (double)dimension_count)> nodes;

		// For every node
		for(std::size_t node = 0; node < nodes.size(); node++){
			// The node's position is seed_type because it's used as a seed
			std::array<seed_type, dimension_count + 1> node_position;

			// Use the private seed as an extra dimension
			node_position[dimension_count] = m_seed * 2;

			// For every dimension
			for(std::size_t dimension = 0; dimension < dimension_count; dimension++){
				// Get the n-dimensional position of the node
				node_position[dimension] = (node / (int)pow(4, (double)dimension)) % 4 + unit_position[dimension];
			}

			// Make a seed sequence from the node position
			std::seed_seq seq(node_position.begin(), node_position.end());

			// An array with only one value lol
			std::array<seed_type, 1> node_seed;

			// Get the node seed
			seq.generate(node_seed.begin(), node_seed.end());

			// Seed the engine with the final node seed
			engine_type engine(node_seed[0]);

			// Escape from zero-land
			engine.discard(discard_count);

			// Get node value
			nodes[node] = distribution_type(-1.0, 1.0)(engine);
		}

		// For every dimension
		for(std::size_t dimension = 0; dimension < dimension_count; dimension++){
			const auto interpolated_node_count = (nodes.size() / 4) / (int)pow(4, (double)dimension);
			for(std::size_t interpolated_node = 0; interpolated_node < interpolated_node_count; interpolated_node++){
				// The node that will be overwritten with the interpolated node (every fourth node is overwritter)
				auto node = interpolated_node * 4;

				// Overwrite every 4th node with an interpolation of the 3 proceding nodes including the overwritten node
				nodes[interpolated_node] = cerp(nodes[node], nodes[node + 1], nodes[node + 2], nodes[node + 3], offset_position[dimension]);
			}
		}

		// The final noise value
		return nodes[0];
	}

	// Return a cubic interpolation of y
	static constexpr auto cerp(float_type y0, float_type y1, float_type y2, float_type y3, float_type mu){
		float_type a0 = y3 - y2 - y0 + y1, a1 = y0 - y1 - a0, a2 = y2 - y0, a3 = y1, mu2 = mu * mu;
		return a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3;
	}
private:
	// The seed of the gradient noise
	seed_type m_seed;
};
};
