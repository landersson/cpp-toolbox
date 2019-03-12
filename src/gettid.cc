
#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>

int gettid()
{
#ifdef __linux__
    return syscall(__NR_gettid);
#else
    //    return reinterpret_cast<int>(pthread_self());
    return syscall(SYS_thread_selfid);
//    return -1;
#endif
}
