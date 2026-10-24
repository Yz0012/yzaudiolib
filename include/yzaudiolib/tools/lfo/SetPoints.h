//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

#pragma once

#include "LFOPoint.h"

template <typename T>
class SetPoints
{
public:
	virtual int addPoint(const LFOPoint<T>& point) = 0;
	virtual int deletePoint(int pointIndex) = 0;
	virtual int movePoints(int pointIndex, T x, T y) = 0;
	virtual int moveControlPoints(int pointIndex, T x, T y) = 0;
private:

};
