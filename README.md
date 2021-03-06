# Gradient Noise
## Gradient noise with a templated number of dimensions

[Animation of 3D gradient noise](http://i.imgur.com/bvJ1E7h.mp4)

[Image of 2D gradient noise](http://i.imgur.com/RO3qT7k.png)

[Image of 1D gradient noise](http://i.imgur.com/iClvq3j.png)
  
### How it works
Gradient noise is generated by cubically interpolating pseudorandom nodes. The nodes have n-dimensional integer positions, but noise values are generated from n-dimensional floating-point positions. Generating gradient noise from a position starts with getting the `unit_position` and the `unit_offset`. The `unit_position` is a copy of the position, but all the axis are floored and casted to an integer. The `unit_offset` is the offset of the `position` from the `unit_position`. Next, the nodes are generated. Nodes are pseudorandom floating-point values between -1 and 1 which are associated with an n-dimensional position. To get a unique pseudorandom number from an n-dimensional position, a `std::seed_seq` is used. The `std::seed_seq` is initialized with each of the axis positions. After the nodes are generated, they are interpolated one dimension at a time using the cubic interpolation function. For n dimensions, 4^n nodes are required, and 4^(n - 1) cubic interpolations are required. The value returned from the final cubic interpolation is the final noise value.

### Why it's useful
Gradient noise is useful for a wide variety of applications including procedural content generation, simulations, and graphical effects.
  
### How to use it
To generate gradient noise, create a `gradient_noise` object, then use the `operator()` method to generate noise values. Returned noise values are between -1 and 1, but due to how cubic interpolation works, values slightly outside of this range are possible.
`gradient_noise` takes two template parameters. The first template parameter, `float_type`, determines the return type and parameter type. The second template parameter, `dimension_count`, determines the number of dimensions. `gradient_noise` is declared inside the `gnd` namespace and can optionally be initialized with a seed. Construction might look like this: `gnd::gradient_noise<my_float_type, my_dimension_count> my_gradient_noise(my_seed)`. If no seed is provided, a default seed is used. After creation, `gradient_noise` can be reseeded using the `seed` method. *Example code below.*
```cpp
// Create a 3D gradient noise engine using 42 as a seed
gnd::gradient_noise<float, 3> gradientNoise3d(42);

// Print a gradient noise value using (123, 555, 777) as a position
std::cout << gradientNoise3d({123.0f, 555.0f, 777.0f}) << std::endl; 

// Reseed the gradient noise engine using 9001 as a new seed
gradientNoise3d.seed(9001); 

// Print a new gradient noise value using the same position as last time
std::cout << gradientNoise3d({123.0f, 555.0f, 777.0f}) << std::endl; 

// Create a 4D gradient noise engine using 1337 as a seed
gnd::gradient_noise<double, 4> gradientNoise4d(1337); 

// Print a gradient noise value using (123, 555, 777, 999) as a position
std::cout << gradientNoise4d({123.0, 555.0, 777.0, 999.0}) << std::endl;
```

### Notes
`gnd::gradient_noise` is designed to be consistent with `std::default_random_engine`. They both use `operator()` overloads to generate values, they can both be reseeded with a `seed` method, and they can both be constructed with seeds optionally. `gnd::gradient_noise` uses `std::default_random_engine::result_type` as the seed type and `std::default_random_engine::default_seed` as the default seed.
