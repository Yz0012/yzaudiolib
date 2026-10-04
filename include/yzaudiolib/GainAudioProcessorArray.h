//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

#include "IYAudioProcessor.h"

#if defined(__AVX2__)
#include <immintrin.h>
#define Y_SIMD_AVX2 1
#elif defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>
#define Y_SIMD_NEON 1
#elif defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#include <emmintrin.h>
#define Y_SIMD_SSE2 1
#endif

enum ParamIndex : int {
	kGain = 0,
	kNumParams
};

template <typename T>
class GainAudioProcessor : public IYAudioProcessor<T>
{

public:
	GainAudioProcessor();
	~GainAudioProcessor();
	/**
	* @brief 对多通道音频缓冲区应用逐样本增益
	* 编译时启用了 AVX2、NEON 或 SSE2，则会使用对应 SIMD 指令加速
	*
	* @param input 输入通道缓冲区数组，长度为 numChannels
	* @param output 输出通道缓冲区数组，长度为 numChannels
	* @param numSamples 每个通道需要处理的采样点数
	* @param numChannels 通道数
	*
	* @note input 与 output 可以相同，以支持原地处理
	* @note 调用前应通过 setParams() 设置有效参数；
	*       params[kGain] 指向的数组长度应与 numSamples 一致
	*/
	void process(T** input, T** output, const int numSamples, const int numChannels) override;
	/**
	* @brief 重置处理器内部状态
	*/
	void reset() override;
	/**
	* @brief 设置处理器参数
	*
	* 枚举列表:
	* 	kGain = 0	// 逐样本增益数组指针，长度与 numSamples 匹配 <br>
	* 	kNumParams	// 参数总数
	*
	* @note 调用者需保证 params 指向的内存在处理器使用期间保持有效
	*/
	void setParams(const double** params) override;
private:
	double initGain[1] = { 1. };
	const double* initParams[1] = { initGain };
	const double* params = nullptr;
};

template <typename T>
GainAudioProcessor<T>::GainAudioProcessor()
{
	this->params = initParams[kGain];
}

template <typename T>
GainAudioProcessor<T>::~GainAudioProcessor() {}

template <typename T>
void GainAudioProcessor<T>::process(T** input, T** output, const int numSamples, const int numChannels)
{
	const double* gainBuf = params;

	for (int ch = 0; ch < numChannels; ++ch)
	{
		const T* in = input[ch];
		T* out = output[ch];
		int i = 0;

		if constexpr (std::is_same_v<T, float>)
		{
#if defined(Y_SIMD_AVX2)
			for (; i + 8 <= numSamples; i += 8)
			{
				__m256 v = _mm256_loadu_ps(in + i);
				__m256d g0 = _mm256_loadu_pd(gainBuf + i);
				__m256d g1 = _mm256_loadu_pd(gainBuf + i + 4);
				__m256 vg = _mm256_insertf128_ps(
					_mm256_castps128_ps256(_mm256_cvtpd_ps(g0)),
					_mm256_cvtpd_ps(g1), 1);
				v = _mm256_mul_ps(v, vg);
				_mm256_storeu_ps(out + i, v);
			}
#elif defined(Y_SIMD_NEON)
			for (; i + 4 <= numSamples; i += 4)
			{
				float32x4_t v = vld1q_f32(in + i);
				float64x2_t g0 = vld1q_f64(gainBuf + i);
				float64x2_t g1 = vld1q_f64(gainBuf + i + 2);
				float32x4_t vg = vcombine_f32(vcvt_f32_f64(g0), vcvt_f32_f64(g1));
				v = vmulq_f32(v, vg);
				vst1q_f32(out + i, v);
			}
#elif defined(Y_SIMD_SSE2)
			for (; i + 4 <= numSamples; i += 4)
			{
				__m128 v = _mm_loadu_ps(in + i);
				__m128d g0 = _mm_loadu_pd(gainBuf + i);
				__m128d g1 = _mm_loadu_pd(gainBuf + i + 2);
				__m128 vg = _mm_movelh_ps(_mm_cvtpd_ps(g0), _mm_cvtpd_ps(g1));
				v = _mm_mul_ps(v, vg);
				_mm_storeu_ps(out + i, v);
			}
#endif
		}
		else if constexpr (std::is_same_v<T, double>)
		{
#if defined(Y_SIMD_AVX2)
			for (; i + 4 <= numSamples; i += 4)
			{
				__m256d v = _mm256_loadu_pd(in + i);
				__m256d vg = _mm256_loadu_pd(gainBuf + i);
				v = _mm256_mul_pd(v, vg);
				_mm256_storeu_pd(out + i, v);
			}
#elif defined(Y_SIMD_NEON)
			for (; i + 2 <= numSamples; i += 2)
			{
				float64x2_t v = vld1q_f64(in + i);
				float64x2_t vg = vld1q_f64(gainBuf + i);
				v = vmulq_f64(v, vg);
				vst1q_f64(out + i, v);
			}
#elif defined(Y_SIMD_SSE2)
			for (; i + 2 <= numSamples; i += 2)
			{
				__m128d v = _mm_loadu_pd(in + i);
				__m128d vg = _mm_loadu_pd(gainBuf + i);
				v = _mm_mul_pd(v, vg);
				_mm_storeu_pd(out + i, v);
			}
#endif
		}

		for (; i < numSamples; ++i)
			out[i] = static_cast<T>(in[i] * static_cast<T>(gainBuf[i]));
	}
}

template <typename T>
void GainAudioProcessor<T>::reset() {}

// 这里不会做任何检查，确保调用者传入的参数数组长度与 numSamples 一致
template <typename T>
void GainAudioProcessor<T>::setParams(const double** params)
{
	this->params = params[kGain];
}