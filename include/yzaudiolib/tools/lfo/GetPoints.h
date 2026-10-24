//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

#pragma once

#include "LFOPoint.h"

template <typename T>
class GetPoints
{
public:
	virtual const std::vector<LFOPoint<T>>& points() const noexcept = 0;
	virtual const std::vector<LFOPoint<T>>& controlPoints() const noexcept = 0;
private:

};
