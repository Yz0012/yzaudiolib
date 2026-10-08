//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

#pragma once

#include "yzaudiolib/IYAudioProcessor.h"

#include "yzaudiolib/audio/receive_vst3_audio.h"
#include "yzaudiolib/params/array_params_pointer.h"

#include <type_traits>

/*
* 组合接口
*/
template <typename T>
class IGainProcessor : public ReceiveVst3Audio<T>, public ParamsArrayPointer
{
public:
	virtual ~IGainProcessor() = default;
private:

};
