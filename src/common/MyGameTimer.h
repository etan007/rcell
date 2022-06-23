#pragma once

#include "xDefine.h"
#include "MinHeap.h"



// 集成定时器
class MyGameTimer
{
public:
    MyGameTimer() {}
    ~MyGameTimer() {}

private:
    // 最小堆节点
    struct MinHeapTimerNode
    {
        // time(nullptr) + u32OnceTimeLen
        UInt64 u4HeapKey = 0;
        // 
        UInt64 u64TimerID = 0;
        UInt64 u64RunCount = 0;
        UInt64 u64TimerData = 0;
        UInt32 u32OnceTimeLen = 0;// 单位 s

    public:
        MinHeapTimerNode(){}
        MinHeapTimerNode(UInt64 _u4HeapKey, UInt64 _u64TimerData, UInt32 _u32OnceTimeLen, UInt64 _u64RunCount, UInt64 _u64TimerID) {
            u4HeapKey = _u4HeapKey;
            u32OnceTimeLen = _u32OnceTimeLen;
            u64RunCount = _u64RunCount;
            u64TimerID = _u64TimerID;
            u64TimerData = _u64TimerData;
        }
        MinHeapTimerNode &operator=(const MinHeapTimerNode &in) {
            if (this == &in) {
                return *this;
            }
            u4HeapKey = in.u4HeapKey;
            u64TimerID = in.u64TimerID;
            u64RunCount = in.u64RunCount;
            u32OnceTimeLen = in.u32OnceTimeLen;
            return *this;
        }
        bool operator()(const MinHeapTimerNode& left, const MinHeapTimerNode& right) const {
            return left.u4HeapKey < right.u4HeapKey;
        }
        void ResetKey() {
            u4HeapKey = time(nullptr) + u32OnceTimeLen;
        }
    };

private:
    mutex mLock;
    // 定时器模块
    MinHeap<MinHeapTimerNode, MinHeapTimerNode> m_mapTimers;

private:
    // 定时器唯一ID
    UInt64 gsTimerID;

private:
    // 这里来管理定时器是否失效
    typedef map<UInt64, UInt64> TIMER_NODE_MAP;
    TIMER_NODE_MAP m_mapTimerNode;

public:
    // 添加定时器节点 【线程安全】
    // timerData    - 定时器附带的数据
    // timerOnceLen - 单次定时器触发的间隔 单位秒
    // runCount        - 定时器触发的次数 0表示一直循环
    // return 返回添加的定时器ID 用于删除操作 和 触发
    UInt64 AddTimer(UInt64 timerData, UInt64 timerOnceLen, UInt64 runCount=1) {
        MinHeapTimerNode stNode(timerOnceLen + time(nullptr), timerData, timerOnceLen, runCount, gsTimerID++);
        mLock.lock();
        m_mapTimers.Push(stNode);
        m_mapTimerNode[stNode.u64TimerID] = 1;
        mLock.unlock();
        return stNode.u64TimerID;
    }
    // 删除定时器 【线程安全】
    // timerID 定时器ID
    // return 如果存在返回true 不存在返回false
    bool RemoveTimer(UInt64 timerID) {
        mLock.lock();
        auto it = m_mapTimerNode.find(timerID);
        if (it == m_mapTimerNode.end()) {
            mLock.unlock();
            return false;
        }

        m_mapTimerNode.erase(it);
        mLock.unlock();
        return true;
    }
    // 清除所有的定时器节点 【线程安全】
    void CleanTimer() {
        mLock.lock();
        while (m_mapTimers.Size() > 0) {
            m_mapTimers.Pop();
        }
        mLock.unlock();
    }

private:
    UInt64 AddTimerEx(const MinHeapTimerNode &stNode) {
        m_mapTimers.Push(stNode);
        return stNode.u64TimerID;
    }

public:
    // 定时器超时回调接口
    virtual void onTimer(UInt64 timerID, UInt64 timerData) = 0;

public:
    void OnHeartBeat(UInt64 timet) {
        mLock.lock();

        // 定时器处理
        do
        {
            if (m_mapTimers.Size() <= 0) {
                break;
            }

            MinHeapTimerNode &stTimerNode = m_mapTimers.Top();

            // 失效定时器处理
            auto it = m_mapTimerNode.find(stTimerNode.u64TimerID);
            if (it == m_mapTimerNode.end()) {
                m_mapTimers.Pop();
                continue;
            }

            if (timet >= stTimerNode.u4HeapKey) {

                // 触发定时器
                onTimer(stTimerNode.u64TimerID, stTimerNode.u64TimerData);

                // 这里删除了后面重新加入
                m_mapTimers.Pop();

                // 最后一次定时器了
                if (1 == stTimerNode.u64RunCount) {
                    //
                    // 删除定时器
                    m_mapTimerNode.erase(stTimerNode.u64TimerID);
                    continue;
                }

                if (stTimerNode.u64RunCount > 0) {
                    stTimerNode.u64RunCount--;
                }

                // 重新放入定时器管理器
                stTimerNode.ResetKey();
                AddTimerEx(stTimerNode);
            }
            else {
                break;
            }
        } while (true);

        mLock.unlock();
    }
};
