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

	/**
	* \~chinese
	* @brief 设置处理器参数
	*
	* 枚举列表:
	* @see @ref yzaudiolib::params::gainarray::ParamIndex
	*
	* @note 无多线程处理
	* 返回数组指针，默认返回gainValues，越界返回gainValues
	*/
	virtual double* params(int) = 0;
private:

};
