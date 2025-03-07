#pragma once
#include <complex>

class FractalInterface
{
public:
	virtual double Evaluate(std::complex<double> c) = 0;
	void SetMaxIterations(int maxIterations);

	static long long cnt;
protected:
	int maxIterations_ = 1000;
};