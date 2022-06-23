#pragma once

#include "xDefine.h"
#include "MinHeap.h"
#include "MyGameTimer.h"

// 操作CD管理器
class GameUserOptionCD
{
public:
    GameUserOptionCD(){}
    ~GameUserOptionCD(){}

private:
    typedef map<string, UInt64> OptionCDMap;
    OptionCDMap m_mapOptionCD;

public:
    void CleanOptionCD() {
        m_mapOptionCD.clear();
    }
    void SetOptionCD(const string &cdKey, UInt32 cdLen) {
        if (cdKey.empty()) {
            return;
        }

        m_mapOptionCD[cdKey] = time(nullptr) + cdLen;
    }
    bool CheckOptionCD(const string &cdKey) {
        if (m_mapOptionCD.find(cdKey) == m_mapOptionCD.end()) {
            return true;
        }

        if ((UInt64)time(nullptr) >= m_mapOptionCD[cdKey]) {
            m_mapOptionCD.erase(cdKey);
            return true;
        }

        return false;
    }
};

// 这里提供玩家的基础工具内容
class GameUserBase : public MyGameTimer, public GameUserOptionCD
{
public:
    GameUserBase(){}
    ~GameUserBase(){}
       
public:
    void OnHeartHeat(UInt64 timet) {
        // 定时器
        MyGameTimer::OnHeartBeat(timet);
    }
};
