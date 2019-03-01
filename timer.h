
#include "gettime.h"

class Timer
{
    uint64_t _t0;

public:
    Timer(bool do_start = true)
    {
        if (do_start)
            reset();
    }

    // reset timer to zero
    void reset()
    {
        _t0 = get_uptime_us()
    }

    uint64_t us_elapsed() const
    {
        return get_uptime_us() - _t0;
    }
};

