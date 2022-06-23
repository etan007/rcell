#pragma once

#include "xDefine.h"
#include <time.h>
#ifndef _WIN32
#include <sys/time.h>
#endif
#include <mutex>

class GUIDMaker
{
public:
    static std::mutex _MakeUInt64ID_Lock;
    static UInt64 MakeUInt64ID(UInt64 rKey) {
        _MakeUInt64ID_Lock.lock();

        struct tm tmT;
        time_t _t = time(NULL);
#ifndef _WIN32
        localtime_r(&_t, &tmT);
#else
        tmT = *localtime(&_t);
#endif
        struct _GUID
        {
            UInt64 r2 : 16; 
            UInt64 r1 : 16; 
            UInt64 sec : 6; 
            UInt64 min : 6; 
            UInt64 hour : 4; 
            UInt64 day : 5; 
            UInt64 month : 4; 
            UInt64 year : 7; 
        }guid;
        guid.year = (tmT.tm_year + 1900) / 62;
        guid.month = tmT.tm_mon + 1;
        guid.day = tmT.tm_mday;
        guid.hour = tmT.tm_hour;
        guid.min = tmT.tm_min;
        guid.sec = tmT.tm_sec;

        static UInt64 rKeyCount = 1;
        ++rKeyCount;
        srand(rKey + _t);
        guid.r1 = (rand()+ rKeyCount) % 0xFFFF;

#ifdef _WIN32
        DWORD m_CurrentTime = GetTickCount() + rKeyCount;
#else
        struct timeval _tstart, _tend;
        struct timezone tz;
        gettimeofday(&_tend, &tz);
        double t1, t2;
        t1 = (double)_tstart.tv_sec * 1000 + (double)_tstart.tv_usec / 1000;
        t2 = (double)_tend.tv_sec * 1000 + (double)_tend.tv_usec / 1000;
        long long m_CurrentTime = (t2 - t1) + rKeyCount;
#endif
        guid.r2 =  m_CurrentTime % 0xFFFF;
        UInt64 retI64 = 0;
        memcpy(&retI64,&guid,sizeof(UInt64));

        _MakeUInt64ID_Lock.unlock();

        // 去掉最高位
        return retI64 & (UInt64)0x7FFFFFFFFFFFFFFF;
    }
};

