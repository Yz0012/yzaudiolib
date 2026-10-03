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
	* @brief 对多通道音频缓冲区应用增益
	*
	* 对每个通道执行 out[i] = in[i] * gain。若 T 为 float 或 double，
	* 且编译时启用了 AVX2、NEON 或 SSE2，则会使用对应 SIMD 指令加速
	*
	* @param input 输入通道缓冲区数组，长度为 numChannels
	* @param output 输出通道缓冲区数组，长度为 numChannels
	* @param numSamples 每个通道需要处理的采样点数
	* @param numChannels 通道数
	*
	* @note input 与 output 可以相同，以支持原地处理
	* @note 调用前应通过 setParams() 设置有效参数，增益值来自 params[kGain]
	*/
	void process(T** input, T** output,const int numSamples, const int numChannels) override;
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
    * @param params 指向参数数组的指针，数组至少包含 kNumParams 个 double。
    *               其中 params[kGain] 表示增益系数
    *
    * @note 调用者需保证 params 指向的内存在处理器使用期间保持有效
    */
	void setParams(const double* params) override;
private:
	const double initParams = { 1.0 };
	const double* params = nullptr;
};

template <typename T>
GainAudioProcessor<T>::GainAudioProcessor()
{
	this->params = &initParams;
}

template <typename T>
GainAudioProcessor<T>::~GainAudioProcessor() {}

template <typename T>
void GainAudioProcessor<T>::process(T** input, T** output,const int numSamples, const int numChannels)
{
	const T gain = static_cast<T>(params[kGain]);

	for (int ch = 0; ch < numChannels; ++ch)
	{
		const T* in = input[ch];
		T* out = output[ch];
		int i = 0;

		if constexpr (std::is_same_v<T, float>)
		{
#if defined(Y_SIMD_AVX2)
			const __m256 vg = _mm256_set1_ps(gain);
			for (; i + 8 <= numSamples; i += 8)
			{
				__m256 v = _mm256_loadu_ps(in + i);
				v = _mm256_mul_ps(v, vg);
				_mm256_storeu_ps(out + i, v);
			}
#elif defined(Y_SIMD_NEON)
			const float32x4_t vg = vdupq_n_f32(gain);
			for (; i + 4 <= numSamples; i += 4)
			{
				float32x4_t v = vld1q_f32(in + i);
				v = vmulq_f32(v, vg);
				vst1q_f32(out + i, v);
			}
#elif defined(Y_SIMD_SSE2)
			const __m128 vg = _mm_set1_ps(gain);
			for (; i + 4 <= numSamples; i += 4)
			{
				__m128 v = _mm_loadu_ps(in + i);
				v = _mm_mul_ps(v, vg);
				_mm_storeu_ps(out + i, v);
			}
#endif
		}
		else if constexpr (std::is_same_v<T, double>)
		{
#if defined(Y_SIMD_AVX2)
			const __m256d vg = _mm256_set1_pd(gain);
			for (; i + 4 <= numSamples; i += 4)
			{
				__m256d v = _mm256_loadu_pd(in + i);
				v = _mm256_mul_pd(v, vg);
				_mm256_storeu_pd(out + i, v);
			}
#elif defined(Y_SIMD_NEON)
			const float64x2_t vg = vdupq_n_f64(gain);
			for (; i + 2 <= numSamples; i += 2)
			{
				float64x2_t v = vld1q_f64(in + i);
				v = vmulq_f64(v, vg);
				vst1q_f64(out + i, v);
			}
#elif defined(Y_SIMD_SSE2)
			const __m128d vg = _mm_set1_pd(gain);
			for (; i + 2 <= numSamples; i += 2)
			{
				__m128d v = _mm_loadu_pd(in + i);
				v = _mm_mul_pd(v, vg);
				_mm_storeu_pd(out + i, v);
			}
#endif
		}

		for (; i < numSamples; ++i)
			out[i] = in[i] * gain;
	}
}

template <typename T>
void GainAudioProcessor<T>::reset() {}

template <typename T>
void GainAudioProcessor<T>::setParams(const double* params)
{
	this->params = params;
}