#pragma once

#include "Renderer/FractalInterface.h"
#include <complex>

/*
This class contains the mathematical implementation of the mandelbrot set.
For more details about this fractal, see: https://en.wikipedia.org/wiki/Mandelbrot_set
*/

class CosFractal : public FractalInterface
{
public:
	double Evaluate(double real, double imaginary)
	{
		std::complex<double> c(real, imaginary);
		if (std::abs(c) < 0.01)
			return 1;
		// At a high level, compute (zn+1 = zn ^ 2 + c) until zn+1 has a magnitude > 2 or until max iterations
		std::complex<double> z = c;
		for (double i = 0; i < maxIterations_; i++) {
			// Calculate zn+1 as zn ^ 2 + c
			
			z = std::cos(z);

			// Check if the magnitude of zn is greater than 2
			// Which is equal to if the square magnitude is greater than 4 (more efficient)
			if (std::abs(z) > 20 * std::abs(c)) {
				return (i / maxIterations_) * 0.5;
			}
		}

		// If after maxIterations the point did not return, it is in the set
		return 0.5 + std::min(0.5, std::abs(z) / (20 * std::abs(c)));
	}
};