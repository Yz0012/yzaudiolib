//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

#include "IYAudioProcessor.h"

enum ParamIndex : int {
	kGain = 0,
	kNumParams
};

template <typename T>
class GainAudioProcessorNoSimd : public IYAudioProcessor<T>
{

public:
	GainAudioProcessorNoSimd();
	~GainAudioProcessorNoSimd();
	/**
	* @brief 对多通道音频缓冲区应用增益
	*
	* 对每个通道执行 out[i] = in[i] * gain，其中增益值在每个采样点都从
	* params[kGain] 重新读取一次，因此当外部传入的参数连续变化时，
	* 输出增益也随之连续变化，无需插值
	*
	* @param input 输入通道缓冲区数组，长度为 numChannels
	* @param output 输出通道缓冲区数组，长度为 numChannels
	* @param numSamples 每个通道需要处理的采样点数
	* @param numChannels 通道数
	*
	* @note input 与 output 可以相同，以支持原地处理
	* @note 调用前应通过 setParams() 设置有效参数，增益值来自 params[kGain]
	* @note 参数缓冲区需在处理期间保持有效，若由其他线程写入需自行保证同步
	*/
	void process(T** input, T** output, const int numSamples, const int numChannels) override;
	/**
	* @brief 重置处理器内部状态
	*
	* 当前 GainAudioProcessor 没有需要重置的状态，因此为空实现
	*/
	void reset() override;
	/**
	* @brief 设置处理器参数
	*
	* 枚举列表:
	* 	kGain = 0	// 增益系数 <br>
	* 	kNumParams	// 参数总数
	*
	* @param params 指向参数数组的指针，数组至少包含 kNumParams 个 double
	*               其中 params[kGain] 表示增益系数
	*
	* @note 调用者需保证 params 指向的内存在处理器使用期间保持有效
	* @note 参数可被外部连续更新，处理器会在每个采样点读取当前值
	*/
	void setParams(const double* params) override;
private:
	const double initParams = { 1.0 };
	const double* params = nullptr;
};

template <typename T>
GainAudioProcessorNoSimd<T>::GainAudioProcessorNoSimd()
{
	this->params = &initParams;
}

template <typename T>
GainAudioProcessorNoSimd<T>::~GainAudioProcessorNoSimd() {}

template <typename T>
void GainAudioProcessorNoSimd<T>::process(T** input, T** output, const int numSamples, const int numChannels)
{
	for (int ch = 0; ch < numChannels; ++ch)
	{
		const T* in = input[ch];
		T* out = output[ch];

		for (int i = 0; i < numSamples; ++i)
		{
			const T gain = static_cast<T>(params[kGain]);
			out[i] = in[i] * gain;
		}
	}
}

template <typename T>
void GainAudioProcessorNoSimd<T>::reset() {}

template <typename T>
void GainAudioProcessorNoSimd<T>::setParams(const double* params)
{
	this->params = params;
}