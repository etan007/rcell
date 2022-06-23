#pragma once
#include <unordered_map>
#include "xSingleton.h"
#include <atomic>

//注意：非线程安全
//本地的防重复ID池
class LocalIDPool : public xSingleton<LocalIDPool>
{
public:
    static uint64_t GetNextId()
    {
        return getMe()._GetNextId();
    }

protected:
    uint64_t _GetNextId()
    {
        static uint64_t s_id = 0;
        return ++s_id;
    }
};
