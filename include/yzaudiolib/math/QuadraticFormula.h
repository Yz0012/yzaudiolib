//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

#pragma once

#include <cmath>

namespace yzaudiolib::math::quadraticformula
{
	/*
	* \~english
	*@returns the number of real roots
	*@params
	*	a: coefficient of x^2
	*	b: coefficient of x
	*	c: constant term
	*	root1: first root (if exists)
	*	root2: second root (if exists)
	*/
	template <typename T>
	inline int solveQuadratic(T a, T b, T c, T& root1, T& root2)
	{
		const double eps = 1e-12;
		if (std::abs(a) < eps)
		{
			if (std::abs(b) < eps)
			{
				return 0;
			}
			else
			{
				root1 = root2 = -c / b;
				return 1;
			}
		}
		T discriminant = b * b - 4 * a * c;
		if (discriminant < -eps)
		{
			return 0;
		}
		else if (std::abs(discriminant) < eps)
		{
			root1 = root2 = -b / (2 * a);
			return 1;
		}
		else
		{
			T sqrtDiscriminant = std::sqrt(discriminant);
			root1 = (-b + sqrtDiscriminant) / (2 * a);
			root2 = (-b - sqrtDiscriminant) / (2 * a);
			return 2;
		}
	}
}