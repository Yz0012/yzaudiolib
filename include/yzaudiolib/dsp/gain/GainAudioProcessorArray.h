//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

#pragma once
#include "yzaudiolib/dsp/gain/IGainProcessor.h"

#include "yzaudiolib/errorcode/error_code_261007.h"
#include "yzaudiolib/kernels/gain_kernels.h"
#include "yzaudiolib/params/params_list/gain_array_params_list.h"

#include <stdexcept>
#include <memory>
#include <cassert>
#include <algorithm>
#include <type_traits>

template <typename T>
class GainAudioProcessorArray : public IGainProcessor<T>
{
	static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>,
		"GainAudioProcessorArray only supports float/double");

	static constexpr int kHardLimit = 1 << 15;

public:
	explicit GainAudioProcessorArray(int maxSamplesPerBlock);
	~GainAudioProcessorArray() override;

	GainAudioProcessorArray(const GainAudioProcessorArray&) = delete;
	GainAudioProcessorArray& operator=(const GainAudioProcessorArray&) = delete;
	GainAudioProcessorArray(GainAudioProcessorArray&&) = delete;
	GainAudioProcessorArray& operator=(GainAudioProcessorArray&&) = delete;

	/**
	* @brief 对多通道音频缓冲区应用逐样本增益
	*
	* @param input 输入通道缓冲区数组，长度为 numChannels
	* @param output 输出通道缓冲区数组，长度为 numChannels
	* @param numSamples 每个通道需要处理的采样点数
	* @param numChannels 通道数
	*
	* @note 通过 params() 设置有效参数；
	*/
	int process(const T* const* input, T* const* output,
		const int numSamples, const int numChannels) override;
	/**
	* @brief 设置处理器参数
	*
	* 枚举列表:
	* @see @ref yzaudiolib::params::gainarray::ParamIndex
	*
	* @note 无多线程处理
	* @warning 返回数组指针，默认返回gainValues，越界返回gainValues
	*/
	double* params(int) override;

	int maxSamplesPerBlock() const noexcept { return maxSamplesPerBlock_; }
	const double* gainValues() const noexcept { return gainValues_; }

private:

	yzaudiolib::error::code261007::ErrorCode validateProcessArgs(const T* const* input, const T* const* output,
		int numSamples, int numChannels) const noexcept;

	void prepareGainBuffer(int numSamples) noexcept;

	void processOneChannel(const T* in, T* out, int numSamples) noexcept;

	int maxSamplesPerBlock_;

	std::unique_ptr<double[]> initGain_ = nullptr;
	double* gainValues_ = nullptr;

	std::unique_ptr<float[]>  gainFloatBuffer_ = nullptr;
};

template <typename T>
GainAudioProcessorArray<T>::GainAudioProcessorArray(int maxSamplesPerBlock)
	: maxSamplesPerBlock_(maxSamplesPerBlock)
{
	if (maxSamplesPerBlock <= 0) {
		throw std::invalid_argument(
			"GainAudioProcessorArray: maxSamplesPerBlock must be > 0");
	}
	if (maxSamplesPerBlock > kHardLimit) {
		throw std::invalid_argument(
			"GainAudioProcessorArray: maxSamplesPerBlock exceeds hard limit");
	}

	initGain_ = std::make_unique<double[]>(maxSamplesPerBlock);
	std::fill_n(initGain_.get(), maxSamplesPerBlock, 1.0);
	gainValues_ = initGain_.get();

	if constexpr (std::is_same_v<T, float>) {
		gainFloatBuffer_ = std::make_unique<float[]>(maxSamplesPerBlock);
	}
}

template <typename T>
GainAudioProcessorArray<T>::~GainAudioProcessorArray() {}

template <typename T>
yzaudiolib::error::code261007::ErrorCode GainAudioProcessorArray<T>::validateProcessArgs(
	const T* const* input, const T* const* output,
	int numSamples, int numChannels) const noexcept
{
	using yzaudiolib::error::code261007::ErrorCode;

	if (gainValues_ == nullptr) return ErrorCode::nullGainValues;
	if (numChannels <= 0) return ErrorCode::invalidNumChannels;
	if (numSamples == 0) return ErrorCode::success;
	if (input == nullptr) return ErrorCode::nullInput;
	if (output == nullptr) return ErrorCode::nullOutput;

	for (int ch = 0; ch < numChannels; ++ch) {
		if (input[ch] == nullptr) return ErrorCode::nullInputChannel;
		if (output[ch] == nullptr) return ErrorCode::nullOutputChannel;
	}

	if (numSamples < 0) return ErrorCode::negativeNumSamples;
	if (numSamples > maxSamplesPerBlock_) return ErrorCode::numSamplesExceedsMax;

	return ErrorCode::success;
}

template <typename T>
void GainAudioProcessorArray<T>::prepareGainBuffer(int numSamples) noexcept
{
	if constexpr (std::is_same_v<T, float>) {
		yzaudiolib::kernels::gain_kernels::convertDoubleToFloat(
			gainFloatBuffer_.get(), gainValues_,
			static_cast<std::size_t>(numSamples));
	}
}

template <typename T>
void GainAudioProcessorArray<T>::processOneChannel(
	const T* in, T* out, int numSamples) noexcept
{
	if constexpr (std::is_same_v<T, float>) {
		yzaudiolib::kernels::gain_kernels::mulF(out, in, gainFloatBuffer_.get(),
			static_cast<std::size_t>(numSamples));
	}
	else {
		yzaudiolib::kernels::gain_kernels::mulD(out, in, gainValues_,
			static_cast<std::size_t>(numSamples));
	}
}

template <typename T>
int GainAudioProcessorArray<T>::process(
	const T* const* input, T* const* output,
	const int numSamples, const int numChannels)
{
	using yzaudiolib::error::code261007::ErrorCode;

	const ErrorCode err =
		validateProcessArgs(input, output, numSamples, numChannels);

	if (err != ErrorCode::success) {
		return static_cast<int>(err);
	}

	prepareGainBuffer(numSamples);

	for (int ch = 0; ch < numChannels; ++ch) {
		processOneChannel(input[ch], output[ch], numSamples);
	}

	return static_cast<int>(ErrorCode::success);
}

template <typename T>
double* GainAudioProcessorArray<T>::params(int paramIndex)
{
	if (paramIndex == yzaudiolib::params::gainarray::ParamIndex::kGain)
		return this->gainValues_;
	assert(false && "GainAudioProcessorArray: invalid paramIndex");
	return this->gainValues_;
}