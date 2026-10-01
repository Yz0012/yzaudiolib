template <typename T>
class IYAudioProcessor
{
public:
	virtual ~IYAudioProcessor() = default;
	virtual void process(T** input, T** output, int numSamples, int numChannels, const float* params) = 0;
	virtual void reset() = 0;
private:

};