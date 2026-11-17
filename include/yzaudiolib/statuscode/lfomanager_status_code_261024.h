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
        success = -1,
        invalidIndex = -2,
        duplicateX = -3,
        xOutOfRange = -4,
        yOutOfRange = -5,
        protectedPoint = -6,
        xConflict = -7,
        invalidArgument = -8,
        controlPointAndPointDuplicateX = -9
    };
}