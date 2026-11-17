//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

/*
* 方便接收VST3音频数据的接口类
*/
template <typename T>
class ReceiveVst3Audio
{
public:
	virtual ~ReceiveVst3Audio() = default;

	/**
	* \~chinese
	* @brief 对多通道音频缓冲区应用逐样本增益
	*
	* @param input 输入通道缓冲区数组，长度为 numChannels
	* @param output 输出通道缓冲区数组，长度为 numChannels
	* @param numSamples 每个通道需要处理的采样点数
	* @param numChannels 通道数
	*
	* @note 通过 params() 设置有效参数；
	*/

	virtual int process(const T* const* input,
		T* const* output,
		const int numSamples,
		const int numChannels) = 0;
private:

};
