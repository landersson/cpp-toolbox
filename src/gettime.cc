
#include <cpp-toolbox/gettime.h>

#include <time.h>

namespace toolbox {

uint64_t get_uptime_us()
{
#ifdef __linux__
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)(ts.tv_sec * 1000000 + ts.tv_nsec / 1000);
#else
    return 0;
#endif
}

class Timer
{};

} // namespace toolbox
