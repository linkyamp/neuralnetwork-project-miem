#pragma once
#include <iostream>
#include <cmath>
class ActivateFunction
{
public:
	void set();
	void use(double* value, int n);
	void useDer(double* value, int n);
	double useDer(double value);
};

