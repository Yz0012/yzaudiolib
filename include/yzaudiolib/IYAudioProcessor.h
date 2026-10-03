//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

template <typename T>
class IYAudioProcessor
{
public:
	virtual ~IYAudioProcessor() = default;
	virtual void setParams(const double* params) = 0;
	virtual void process(T** input, T** output,const int numSamples, const int numChannels) = 0;
	virtual void reset() = 0;
private:

};