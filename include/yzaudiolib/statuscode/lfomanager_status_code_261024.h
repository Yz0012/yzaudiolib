//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

namespace yzaudiolib::status::code261024
{
    /*
    * 状态码
    */
    enum class LFOStatus : int
    {
        success = 0,
        invalidIndex = -1,
        duplicateX = -2,
        xOutOfRange = -3,
        yOutOfRange = -4,
        protectedPoint = -5,
        xConflict = -6,
        invalidArgument = -7,
        controlPointAndPointDuplicateX = -8
    };
}