#pragma once

#include "xDefine.h"
#include "MinHeap.h"



// ���ɶ�ʱ��
class MyGameTimer
{
public:
    MyGameTimer() {}
    ~MyGameTimer() {}

private:
    // ��С�ѽڵ�
    struct MinHeapTimerNode
    {
        // time(nullptr) + u32OnceTimeLen
        UInt64 u4HeapKey = 0;
        // 
        UInt64 u64TimerID = 0;
        UInt64 u64RunCount = 0;
        UInt64 u64TimerData = 0;
        UInt32 u32OnceTimeLen = 0;// ��λ s

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
    // ��ʱ��ģ��
    MinHeap<MinHeapTimerNode, MinHeapTimerNode> m_mapTimers;

private:
    // ��ʱ��ΨһID
    UInt64 gsTimerID;

private:
    // ����������ʱ���Ƿ�ʧЧ
    typedef map<UInt64, UInt64> TIMER_NODE_MAP;
    TIMER_NODE_MAP m_mapTimerNode;

public:
    // ��Ӷ�ʱ���ڵ� ���̰߳�ȫ��
    // timerData    - ��ʱ������������
    // timerOnceLen - ���ζ�ʱ�������ļ�� ��λ��
    // runCount        - ��ʱ�������Ĵ��� 0��ʾһֱѭ��
    // return ������ӵĶ�ʱ��ID ����ɾ������ �� ����
    UInt64 AddTimer(UInt64 timerData, UInt64 timerOnceLen, UInt64 runCount=1) {
        MinHeapTimerNode stNode(timerOnceLen + time(nullptr), timerData, timerOnceLen, runCount, gsTimerID++);
        mLock.lock();
        m_mapTimers.Push(stNode);
        m_mapTimerNode[stNode.u64TimerID] = 1;
        mLock.unlock();
        return stNode.u64TimerID;
    }
    // ɾ����ʱ�� ���̰߳�ȫ��
    // timerID ��ʱ��ID
    // return ������ڷ���true �����ڷ���false
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
    // ������еĶ�ʱ���ڵ� ���̰߳�ȫ��
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
    // ��ʱ����ʱ�ص��ӿ�
    virtual void onTimer(UInt64 timerID, UInt64 timerData) = 0;

public:
    void OnHeartBeat(UInt64 timet) {
        mLock.lock();

        // ��ʱ������
        do
        {
            if (m_mapTimers.Size() <= 0) {
                break;
            }

            MinHeapTimerNode &stTimerNode = m_mapTimers.Top();

            // ʧЧ��ʱ������
            auto it = m_mapTimerNode.find(stTimerNode.u64TimerID);
            if (it == m_mapTimerNode.end()) {
                m_mapTimers.Pop();
                continue;
            }

            if (timet >= stTimerNode.u4HeapKey) {

                // ������ʱ��
                onTimer(stTimerNode.u64TimerID, stTimerNode.u64TimerData);

                // ����ɾ���˺������¼���
                m_mapTimers.Pop();

                // ���һ�ζ�ʱ����
                if (1 == stTimerNode.u64RunCount) {
                    //
                    // ɾ����ʱ��
                    m_mapTimerNode.erase(stTimerNode.u64TimerID);
                    continue;
                }

                if (stTimerNode.u64RunCount > 0) {
                    stTimerNode.u64RunCount--;
                }

                // ���·��붨ʱ��������
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
