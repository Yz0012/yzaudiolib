//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

#pragma once
#include <type_traits>

class IYAudioProcessor
{
public:
	virtual ~IYAudioProcessor() = default;

	template <typename U>
	const U* dc() const
	{
		static_assert(std::is_base_of_v<IYAudioProcessor, U>,
			"U must be derived from IYAudioProcessor");
		return dynamic_cast<const U*>(this);
	}
};