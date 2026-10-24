//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <vector>
#include <type_traits>
#include <stdexcept>

#include "SetPoints.h"
#include "GetPoints.h"
#include "yzaudiolib/statuscode/lfomanager_status_code_261024.h"

template <typename T>
class LFOPointManager : public SetPoints<T>, public GetPoints<T>
{
	static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>,
		"LFOPointManager only supports float/double");
public:
	using LFOStatus = yzaudiolib::status::code261024::LFOStatus;
	explicit LFOPointManager(T w, T h);

	int addPoint(const LFOPoint<T>& point) override;
	int deletePoint(int pointIndex) override;
	const std::vector<LFOPoint<T>>& points() const noexcept override;
	const std::vector<LFOPoint<T>>& controlPoints() const noexcept override;
	int movePoints(int pointIndex, T x, T y) override;
	int moveControlPoints(int pointIndex, T x, T y) override;

private:
	T width;
	T height;

	std::vector<LFOPoint<T>> points_;
	std::vector<LFOPoint<T>> controlPoints_;

	int searchPointPosition(const std::vector<LFOPoint<T>>& lfoPointArray, T pointX, int& pointIndex);
	int addControlPoint(int pointIndex);
	bool isValidX(int pointIndex, bool isControlPoint, T x) const;
	bool isValidY(T y) const;
};

template <typename T>
inline LFOPointManager<T>::LFOPointManager(T w, T h) : width(w), height(h)
{
	if (width <= 0 || height <= 0)
	{
		throw std::out_of_range("width or height should > 0");
	}
	points_.emplace_back(LFOPoint<T>{ T(0), height });
	points_.emplace_back(LFOPoint<T>{ width, height });
	controlPoints_.emplace_back(LFOPoint<T>{ width / T(2), height });
}

template <typename T>
inline int LFOPointManager<T>::addPoint(const LFOPoint<T>& point)
{
	int pointIndex = 0;
	const int status = searchPointPosition(this->points_, point.x, pointIndex);
	if (status != static_cast<int>(LFOStatus::success))
	{
		return status;
	}

	if (!isValidY(point.y)) { return static_cast<int>(LFOStatus::yOutOfRange); }
	points_.insert(points_.begin() + pointIndex, point);
	const int status2 = addControlPoint(pointIndex);
	if (status2 != static_cast<int>(LFOStatus::success))
	{
		points_.erase(points_.begin() + pointIndex);
		return status2;
	}
	return pointIndex;
}

template <typename T>
inline int LFOPointManager<T>::deletePoint(int pointIndex)
{
	const int n = static_cast<int>(points_.size());

	if (pointIndex < 0 || pointIndex >= n)
	{
		return static_cast<int>(LFOStatus::invalidIndex);
	}

	if (pointIndex == 0 || pointIndex == n - 1)
	{
		return static_cast<int>(LFOStatus::protectedPoint);
	}

	points_.erase(points_.begin() + pointIndex);
	controlPoints_.erase(controlPoints_.begin() + pointIndex);
	return static_cast<int>(LFOStatus::success);
}

template <typename T>
inline int LFOPointManager<T>::searchPointPosition(
	const std::vector<LFOPoint<T>>& lfoPointArray,
	T pointX,
	int& pointIndex)
{
	if (pointX <= lfoPointArray.front().x)
	{
		pointIndex = 1;
		return static_cast<int>(pointX == lfoPointArray.front().x
			? LFOStatus::duplicateX
			: LFOStatus::xOutOfRange);
	}

	if (pointX >= lfoPointArray.back().x)
	{
		pointIndex = static_cast<int>(lfoPointArray.size()) - 1;
		return static_cast<int>(pointX == lfoPointArray.back().x
			? LFOStatus::duplicateX
			: LFOStatus::xOutOfRange);
	}


	int left = 0;
	int right = static_cast<int>(lfoPointArray.size());
	while (left < right)
	{
		int mid = left + (right - left) / 2;
		if (lfoPointArray.at(mid).x <= pointX)
		{
			left = mid + 1;
		}
		else
		{
			right = mid;
		}
	}
	pointIndex = left;

	if (lfoPointArray.at(pointIndex - 1).x == pointX)
	{
		return static_cast<int>(LFOStatus::duplicateX);
	}
	return static_cast<int>(LFOStatus::success);
}

template <typename T>
inline int LFOPointManager<T>::addControlPoint(int pointIndex)
{
	T controlPointX_ = controlPoints_.at(pointIndex - 1).x;
	T pointX_ = points_.at(pointIndex).x;

	if (controlPointX_ == pointX_) return static_cast<int>(LFOStatus::controlPointAndPointDuplicateX);

	if (controlPointX_ > pointX_)
	{
		T controlPointX = (pointX_ + points_.at(pointIndex - 1).x) / T(2);
		T controlPointY = (points_.at(pointIndex).y + points_.at(pointIndex - 1).y) / T(2);
		controlPoints_.emplace(
			controlPoints_.begin() + pointIndex - 1,
			LFOPoint<T>{ controlPointX, controlPointY });
		return static_cast<int>(LFOStatus::success);
	}
	else
	{
		T controlPointX = (pointX_ + points_.at(pointIndex + 1).x) / T(2);
		T controlPointY = (points_.at(pointIndex).y + points_.at(pointIndex + 1).y) / T(2);
		controlPoints_.emplace(
			controlPoints_.begin() + pointIndex,
			LFOPoint<T>{ controlPointX, controlPointY });
		return static_cast<int>(LFOStatus::success);
	}
}

template <typename T>
inline const std::vector<LFOPoint<T>>& LFOPointManager<T>::points() const noexcept
{
	return this->points_;
}

template <typename T>
inline const std::vector<LFOPoint<T>>& LFOPointManager<T>::controlPoints() const noexcept
{
	return this->controlPoints_;
}

template <typename T>
inline bool LFOPointManager<T>::isValidX(int pointIndex, bool isControlPoint, T x) const
{
	const int n = static_cast<int>(points_.size());
	if (isControlPoint)
	{
		if (pointIndex < 0 || pointIndex >= static_cast<int>(controlPoints_.size()))
			return false;

		return points_[pointIndex].x < x && x < points_[pointIndex + 1].x;
	}

	if (pointIndex > 0)
	{
		const T lo = std::max(points_[pointIndex - 1].x,
			controlPoints_[pointIndex - 1].x);
		if (x <= lo)
			return false;
	}

	if (pointIndex < n - 1)
	{
		const T hi = std::min(points_[pointIndex + 1].x,
			controlPoints_[pointIndex].x);
		if (x >= hi)
			return false;
	}

	return true;
}

template <typename T>
inline bool LFOPointManager<T>::isValidY(T y) const
{
	return y >= T(0) && y <= height;
}

template <typename T>
inline int LFOPointManager<T>::movePoints(int pointIndex, T x, T y)
{
	const int n = static_cast<int>(points_.size()) - 1;
	if (pointIndex <= 0 || pointIndex >= n)
		return static_cast<int>(LFOStatus::invalidIndex);

	if (!isValidX(pointIndex, false, x))
	{
		if (!isValidY(y))
		{
			return static_cast<int>(LFOStatus::yOutOfRange);
		}
		else
		{
			points_[pointIndex].y = y;
		}
	}
	else
	{
		points_[pointIndex].x = x;
		if (!isValidY(y))
		{
			return static_cast<int>(LFOStatus::yOutOfRange);
		}
		else
		{
			points_[pointIndex].y = y;
		}
	}

	return static_cast<int>(LFOStatus::success);
}

template <typename T>
inline int LFOPointManager<T>::moveControlPoints(int pointIndex, T x, T y)
{
	const int n = static_cast<int>(controlPoints_.size()) - 1;
	if (pointIndex <= 0 || pointIndex >= n)
		return static_cast<int>(LFOStatus::invalidIndex);

	if (!isValidX(pointIndex, true, x))
	{
		if (!isValidY(y))
		{
			return static_cast<int>(LFOStatus::yOutOfRange);
		}
		else
		{
			controlPoints_[pointIndex].y = y;
		}
	}
	else
	{
		controlPoints_[pointIndex].x = x;
		if (!isValidY(y))
		{
			return static_cast<int>(LFOStatus::yOutOfRange);
		}
		else
		{
			controlPoints_[pointIndex].y = y;
		}
	}

	return static_cast<int>(LFOStatus::success);
}
