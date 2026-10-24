//------------------------------------------------------------------------
// Copyright(c) 2026 Y_z00.
//------------------------------------------------------------------------

#pragma once

#include <chrono>

namespace yzaudiolib::tools::detectDoubleClick
{
    inline int detectDoubleClick(int interval = 500)
    {
        using namespace std::chrono;

        static steady_clock::time_point lastClickTime;
        static bool hasLastClick = false;

        auto now = steady_clock::now();

        if (hasLastClick)
        {
            auto elapsed = duration_cast<milliseconds>(now - lastClickTime).count();
            if (elapsed <= interval)
            {
                hasLastClick = false;
                return 1;
            }
        }

        lastClickTime = now;
        hasLastClick = true;
        return 0;
    }
}