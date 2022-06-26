#include "common.h"

namespace CommonFunc
{
    UInt64 GetTickCount()
    {
#ifdef WIN32
        return ::GetTickCount64();
#else
        UInt64 uTickCount = 0;
        struct timespec on;
        if(0 == clock_gettime(CLOCK_MONOTONIC, &on) )
        {
            uTickCount = on.tv_sec * 1000 + on.tv_nsec / 1000000;
        }

        return uTickCount;
#endif
    }

    void Sleep(UInt64 nMilliseconds)
    {
#ifdef WIN32
        ::Sleep(nMilliseconds);
#else
        struct timespec req;
        req.tv_sec = nMilliseconds / 1000;
        req.tv_nsec = nMilliseconds % 1000 * 1000000;
        nanosleep(&req, NULL);
#endif
        return;
    }
    
}