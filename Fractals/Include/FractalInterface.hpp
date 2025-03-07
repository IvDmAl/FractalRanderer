#pragma once
#include <complex>

class FractalInterface
{
public:
	virtual double Evaluate(std::complex<double> c) = 0;
	void SetMaxIterations(int maxIterations);
	int GetMaxIterations();
protected:
	int maxIterations_ = 500;
};