#pragma once
#include <random> // Include default_random_engine and uniform_real_distribution
#include <array>

namespace gnd{ // "Generic N-Dimensional"
/* float_type is the type of the noise values and the noise parameters
   dimension_count is the number of dimensions the noise has */
template<typename float_type, std::size_t dimension_count>
class gradient_noise{
public:
	typedef std::default_random_engine engine_type; // The type of the engine
	typedef std::uniform_real_distribution<float_type> distribution_type; // The type of the distribution
	typedef engine_type::result_type seed_type; // The type of the seed of the engine and the noise
	static constexpr unsigned int discard_count = 1; // The number of random numbers discarded after a new engine seed
	gradient_noise(engine_type::result_type seed = engine_type::default_seed); // Construct gradient noise with an optional seed
	void seed(seed_type val = engine_type::default_seed); // Seed the gradient noise
	float_type operator()(std::array<float_type, dimension_count> position); // Return a noise value from an n-dimensional position
private:
	static float_type cerp(float_type y0, float_type y1, float_type y2, float_type y3, float_type mu); // Return a cubic interpolation of y
	seed_type m_seed; // The seed of the gradient noise
};
};
