#include "IYAudioProcessor.h"

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
GainAudioProcessor<T>::GainAudioProcessor()
{
}

template <typename T>
GainAudioProcessor<T>::~GainAudioProcessor()
{
}

template <typename T>
void GainAudioProcessor<T>::process(T** input, T** output, int numSamples, int numChannels, const float* params)
{
    T** in = input;
    T** out = output;

    float gain = params[mGain];

    for (int i = 0; i < numChannels; i++)
    {
        T* ptrIn = static_cast<T*>(in[i]);
        T* ptrOut = static_cast<T*>(out[i]);
        int n = numSamples;
        while (--n >= 0)
        {
            (*ptrOut++) = (*ptrIn++) * static_cast<T>(gain);
        }
    }
}

template <typename T>
void GainAudioProcessor<T>::reset()
{
}