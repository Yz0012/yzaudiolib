//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

#pragma once

#include "lfopoint.h"

template <typename T>
class IGetPoints
{
public:
	virtual const LFOPoint<T> originPoint() const noexcept = 0;
	virtual const std::vector<LFOPoint<T>>& points() const noexcept = 0;
	virtual const std::vector<LFOPoint<T>>& controlPoints() const noexcept = 0;
	virtual const T drawPointWidthRange() const noexcept = 0;
	virtual const T drawPointHeightRange() const noexcept = 0;
private:

};
