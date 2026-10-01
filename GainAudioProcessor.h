#include "IYAudioProcessor.h"
#include <type_traits>

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

template <typename T>
class GainAudioProcessor : public IYAudioProcessor<T>
{
    enum ParamIndex : int {
        mGain = 0,
        kNumParams
    };

public:
    GainAudioProcessor();
    ~GainAudioProcessor();
    void process(T** input, T** output, int numSamples, int numChannels, const float* params) override;
    void reset() override;
private:
};

template <typename T>
GainAudioProcessor<T>::GainAudioProcessor() {}

template <typename T>
GainAudioProcessor<T>::~GainAudioProcessor() {}

template <typename T>
void GainAudioProcessor<T>::process(T** input, T** output, int numSamples, int numChannels, const float* params)
{
    const T gain = static_cast<T>(params[mGain]);

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