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
		gradient_noise(engine_type::result_type seed = engine_type::default_seed);

		// Seed the gradient noise
		void seed(seed_type val = engine_type::default_seed);

		// Return a noise value from an n-dimensional position
		float_type operator()(std::array<float_type, dimension_count> position);
	private:
		// Return a cubic interpolation of y
		static float_type cerp(float_type y0, float_type y1, float_type y2, float_type y3, float_type mu);

		// The seed of the gradient noise
		seed_type m_seed;
	};
};
