//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

#pragma once

#include "yzaudiolib/IYAudioProcessor.h"

// 接口
#include "yzaudiolib/audio/receive_vst3_audio.h"
#include "yzaudiolib/params/array_params_pointer.h"

// 模块
#include "yzaudiolib/statuscode/gainarray_status_code_261007.h"
#include "yzaudiolib/dsp/gain/kernels/gain_kernels.h"
#include "yzaudiolib/params/params_list/gain_array_params_list.h"

#include <stdexcept>
#include <memory>
#include <cassert>
#include <algorithm>
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
