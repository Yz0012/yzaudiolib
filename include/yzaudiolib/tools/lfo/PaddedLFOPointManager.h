//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

#pragma once

#include <stdexcept>
#include <vector>

#include "LFOPointManager.h"

template <typename T>
class PaddedLFOPointManager : public LFOPointManager<T>
{
public:
	using Base = LFOPointManager<T>;

	explicit PaddedLFOPointManager(T width, T height, T padding)
		: Base(checkedInner(width, padding),
			checkedInner(height, padding))
		, width_(width)
		, height_(height)
		, padding_(padding)
	{
		syncCache();
	}

	int addPoint(const LFOPoint<T>& point) override
	{
		const LFOPoint<T> internalPoint{
			point.x - padding_,
			point.y - padding_
		};

		const int ret = Base::addPoint(internalPoint);
		syncCache();
		return ret;
	}

	int deletePoint(int pointIndex) override
	{
		const int ret = Base::deletePoint(pointIndex);
		syncCache();
		return ret;
	}

	const LFOPoint<T> originPoint() const noexcept override
	{
		return LFOPoint<T>{T(padding_), T(padding_)};
	}

	const std::vector<LFOPoint<T>>& points() const noexcept override
	{
		return externalPoints_;
	}

	const std::vector<LFOPoint<T>>& controlPoints() const noexcept override
	{
		return externalControlPoints_;
	}

	const T drawPointWidthRange() const noexcept override
	{
		return width_ - 2 * padding_;
	}

	const T drawPointHeightRange() const noexcept override
	{
		return height_ - 2 * padding_;
	}

	int movePoints(int pointIndex, T x, T y) override
	{
		const int ret = Base::movePoints(
			pointIndex,
			x - padding_,
			y - padding_);

		syncCache();
		return ret;
	}

	int moveControlPoints(int pointIndex, T x, T y) override
	{
		const int ret = Base::moveControlPoints(
			pointIndex,
			x - padding_,
			y - padding_);

		syncCache();
		return ret;
	}

	int moveEndPoints(T y) override
	{
		const int ret = Base::moveEndPoints(y - padding_);
		syncCache();
		return ret;
	}

	int reset() override
	{
		const int ret = Base::reset();
		syncCache();
		return ret;
	}

private:
	T width_;
	T height_;
	T padding_;
	std::vector<LFOPoint<T>> externalPoints_;
	std::vector<LFOPoint<T>> externalControlPoints_;

	static T checkedInner(T outer, T padding)
	{
		if (padding < T(0))
		{
			throw std::invalid_argument("padding must be >= 0");
		}
		if (outer <= T(2) * padding)
		{
			throw std::out_of_range("outer size must be > 2 * padding");
		}
		return outer - T(2) * padding;
	}

	void syncCache()
	{
		const auto& internalPoints = Base::points();
		const auto& internalControlPoints = Base::controlPoints();

		externalPoints_.clear();
		externalPoints_.reserve(internalPoints.size());
		for (const auto& p : internalPoints)
		{
			externalPoints_.push_back(LFOPoint<T>{
				p.x + padding_,
					p.y + padding_
			});
		}

		externalControlPoints_.clear();
		externalControlPoints_.reserve(internalControlPoints.size());
		for (const auto& p : internalControlPoints)
		{
			externalControlPoints_.push_back(LFOPoint<T>{
				p.x + padding_,
					p.y + padding_
			});
		}
	}
};