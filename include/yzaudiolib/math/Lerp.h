//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

#pragma once

namespace yzaudiolib::math::lerp
{
	template <typename T>
	inline inline T Lerp(T a, T b, float t) {
		return a + (b - a) * t;
	}
}