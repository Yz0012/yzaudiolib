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

	virtual int process(const T* const* input,
		T* const* output,
		const int numSamples,
		const int numChannels) = 0;
private:

};
