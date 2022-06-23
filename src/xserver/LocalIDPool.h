#pragma once
#include <unordered_map>
#include "xSingleton.h"
#include <atomic>

//ע�⣺���̰߳�ȫ
//���صķ��ظ�ID��
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
