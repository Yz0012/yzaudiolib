//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

namespace yzaudiolib::status::code261007
{
	/*
	* 状态码
	*/
	enum class StatusCode : int
	{
		success = 0,                // 成功
		nullGainValues,         // 内部增益数组 gainValues_ 为空
		nullInput,              // 输入通道缓冲区数组 input 为空
		nullOutput,             // 输出通道缓冲区数组 output 为空
		nullInputChannel,         // 输入通道缓冲区 input[i] 为空
		nullOutputChannel,        // 输出通道缓冲区 output[i] 为空
		negativeNumSamples,     // numSamples < 0
		numSamplesExceedsMax,   // numSamples > maxSamplesPerBlock_
		invalidNumChannels,     // numChannels <= 0
	};

}