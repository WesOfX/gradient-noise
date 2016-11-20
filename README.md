# Gradient Noise
## Gradient noise function with a templated number of dimensions.
To use gradient noise, create a `gradient_noise` object and use the `operator()` overload. `gradient_noise` is declared inside the `gnd` namespace.`gradient_noise` takes two template parameters. The first template parameter, `float_type`, determines the return type and input parameter type of the `gradient_noise`. The second template parameter, `dimension_count`, determins the number of dimensions (the number of input parameters). *Example code below*
```
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

