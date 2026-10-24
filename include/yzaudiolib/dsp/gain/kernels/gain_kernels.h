//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

#pragma once
#include <cstddef>

#if defined(__AVX2__)
#include <immintrin.h>
#define YZ00_HAS_AVX2 1
#elif defined(__aarch64__) && (defined(__ARM_NEON) || defined(__ARM_NEON__))
#include <arm_neon.h>
#define YZ00_HAS_NEON 1
#elif defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#include <emmintrin.h>
#define YZ00_HAS_SSE2 1
#endif

namespace yzaudiolib::kernels::gain_kernels {

    inline void mulScalarF(float* out, const float* a, const float* b, std::size_t n) noexcept {
        for (std::size_t i = 0; i < n; ++i) out[i] = a[i] * b[i];
    }
    inline void mulScalarD(double* out, const double* a, const double* b, std::size_t n) noexcept {
        for (std::size_t i = 0; i < n; ++i) out[i] = a[i] * b[i];
    }

#if defined(YZ00_HAS_AVX2)
    inline std::size_t mulAvx2F(float* out, const float* a, const float* b, std::size_t n) noexcept {
        std::size_t i = 0;
        for (; i + 8 <= n; i += 8) {
            __m256 va = _mm256_loadu_ps(a + i);
            __m256 vb = _mm256_loadu_ps(b + i);
            _mm256_storeu_ps(out + i, _mm256_mul_ps(va, vb));
        }
        return i;
    }
    inline std::size_t mulAvx2D(double* out, const double* a, const double* b, std::size_t n) noexcept {
        std::size_t i = 0;
        for (; i + 4 <= n; i += 4) {
            __m256d va = _mm256_loadu_pd(a + i);
            __m256d vb = _mm256_loadu_pd(b + i);
            _mm256_storeu_pd(out + i, _mm256_mul_pd(va, vb));
        }
        return i;
    }
#endif

#if defined(YZ00_HAS_NEON)
    inline std::size_t mulNeonF(float* out, const float* a, const float* b, std::size_t n) noexcept {
        std::size_t i = 0;
        for (; i + 4 <= n; i += 4) {
            float32x4_t va = vld1q_f32(a + i);
            float32x4_t vb = vld1q_f32(b + i);
            vst1q_f32(out + i, vmulq_f32(va, vb));
        }
        return i;
    }
    inline std::size_t mulNeonD(double* out, const double* a, const double* b, std::size_t n) noexcept {
        std::size_t i = 0;
        for (; i + 2 <= n; i += 2) {
            float64x2_t va = vld1q_f64(a + i);
            float64x2_t vb = vld1q_f64(b + i);
            vst1q_f64(out + i, vmulq_f64(va, vb));
        }
        return i;
    }
#endif

#if defined(YZ00_HAS_SSE2)
    inline std::size_t mulSse2F(float* out, const float* a, const float* b, std::size_t n) noexcept {
        std::size_t i = 0;
        for (; i + 4 <= n; i += 4) {
            __m128 va = _mm_loadu_ps(a + i);
            __m128 vb = _mm_loadu_ps(b + i);
            _mm_storeu_ps(out + i, _mm_mul_ps(va, vb));
        }
        return i;
    }
    inline std::size_t mulSse2D(double* out, const double* a, const double* b, std::size_t n) noexcept {
        std::size_t i = 0;
        for (; i + 2 <= n; i += 2) {
            __m128d va = _mm_loadu_pd(a + i);
            __m128d vb = _mm_loadu_pd(b + i);
            _mm_storeu_pd(out + i, _mm_mul_pd(va, vb));
        }
        return i;
    }
#endif

    inline void mulF(float* out, const float* a, const float* b, std::size_t n) noexcept {
        std::size_t i = 0;
#if defined(YZ00_HAS_AVX2)
        i = mulAvx2F(out, a, b, n);
#elif defined(YZ00_HAS_NEON)
        i = mulNeonF(out, a, b, n);
#elif defined(YZ00_HAS_SSE2)
        i = mulSse2F(out, a, b, n);
#endif
        mulScalarF(out + i, a + i, b + i, n - i);
    }

    inline void mulD(double* out, const double* a, const double* b, std::size_t n) noexcept {
        std::size_t i = 0;
#if defined(YZ00_HAS_AVX2)
        i = mulAvx2D(out, a, b, n);
#elif defined(YZ00_HAS_NEON)
        i = mulNeonD(out, a, b, n);
#elif defined(YZ00_HAS_SSE2)
        i = mulSse2D(out, a, b, n);
#endif
        mulScalarD(out + i, a + i, b + i, n - i);
    }

    inline void convertDoubleToFloat(float* out, const double* in, std::size_t n) noexcept {
        for (std::size_t i = 0; i < n; ++i) out[i] = static_cast<float>(in[i]);
    }

}