//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

/*
* 用于参数数组指针接口
*/
class ParamsArrayPointer
{
public:

	virtual ~ParamsArrayPointer() = default;

	virtual double* params(int) = 0;
private:

};
