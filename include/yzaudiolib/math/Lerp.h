//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

#pragma once

namespace yzaudiolib::math::lerp
{
	/*
	* \~english
	*@returns the linearly interpolated value between a and b
	*@params
	*	a: first value
	*	b: second value
	*	t: interpolation factor (0.0 to 1.0)
	*/
	template <typename T>
	inline inline T lerp(T a, T b, float t) {
		return a + (b - a) * t;
	}
}