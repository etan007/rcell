// ObjPool.h
//
/////////////////////////////////////////////////////

#ifndef __OBJPOOL_H__
#define __OBJPOOL_H__

#include "xDefine.h"

#include <mutex>

class xObjPoolBase
{
public:
    xObjPoolBase() {}
    virtual ~xObjPoolBase() {}

private:
    UInt32 m_u32PoolIndex = 0;

public:
    virtual void cleanUp(){}

public:
    UInt32 GetPoolID() {
        return m_u32PoolIndex;
    }
    void SetPoolID(UInt32 poolIndex) {
        m_u32PoolIndex = poolIndex;
    }
};

template<class T>
class xObjPool
{
public:
    xObjPool(void) {
    }

    ~xObjPool(void) {
        Term();
        //Assert(m_papObj == nullptr);
    }

public:
    bool bInit = false;
    // 保存空闲的节点防止多次归还
    map<T*, int> mFreeMap;

public:
    bool Init(Int32 nMaxCount) {
        //Assert(nMaxCount > 0);
        if (nMaxCount <= 0) {
            return false;
        }

        m_nMaxCount = nMaxCount;
        m_nPosition = 0;
        m_papObj = new T*[m_nMaxCount];

        for (Int32 i = 0; i < m_nMaxCount; i++)
        {
            m_papObj[i] = new T;
            if (!m_papObj[i])
            {
                //Assert(m_papObj[i] != nullptr);
                return false;
            }

            mFreeMap[m_papObj[i]] = 1;
        }

        bInit = true;
        return true;
    }

    void Term(void) {
        if (m_papObj != nullptr)
        {
            for (Int32 i = 0; i < m_nMaxCount; i++)
            {
                if (m_papObj[i]) {
                    delete m_papObj[i];
                    m_papObj[i] = nullptr;
                }
            }

            delete[] m_papObj;
            m_papObj = nullptr;
        }

        m_nMaxCount = -1;
        m_nPosition = -1;

        mFreeMap.clear();
    }

    T* NewObj(void) {
        LOCK_L(m_Lock);
        //Assert(m_nPosition < m_nMaxCount);
        if (m_nPosition >= m_nMaxCount)
        {
            Resize(m_nMaxCount + 1024);
            //m_Lock.unlock();
            //return nullptr;
        }

        T *pObj = m_papObj[m_nPosition];
        pObj->SetPoolID((UInt32)m_nPosition);
        m_nPosition++;

        mFreeMap.erase(pObj);
        UNLOCK_L(m_Lock);
        return pObj;
    }

    bool Resize(Int32 nMaxCount) {
        //Assert(nMaxCount > 0);
        if (nMaxCount <= m_nMaxCount) {
            return false;
        }

        Int32 i32PreMaxCount = m_nMaxCount;
        m_nMaxCount = nMaxCount;
        T **m_papObjT = new T*[m_nMaxCount];

        // 旧的数据设置过来  
        for (Int32 i = 0; i < i32PreMaxCount; i++)
        {
            m_papObjT[i] = m_papObj[i];
        }
        // 新申请的数量  
        for (Int32 i = i32PreMaxCount; i < m_nMaxCount; i++)
        {
            m_papObjT[i] = new T;
            if (m_papObjT[i] == nullptr)
            {
                //Assert(m_papObj[i] != nullptr);
                return false;
            }
            mFreeMap[m_papObjT[i]] = 1;
        }
        delete[]m_papObj;
        m_papObj = m_papObjT;
        return true;
    }

    void DeleteObj(T *pObj) {

        LOCK_L(m_Lock);
        //Assert(pObj != nullptr);
        if (pObj == nullptr)
        {
            UNLOCK_L(m_Lock);
            return;
        }
        // 节点已经在空闲列表中了
        if (mFreeMap.find(pObj)!= mFreeMap.end()) {
            UNLOCK_L(m_Lock);
            return;
        }
        //Assert(m_nPosition > 0);
        if (m_nPosition <= 0)
        {
            UNLOCK_L(m_Lock);
            return;
        }

        UInt32 uDelIndex = pObj->GetPoolID();
        //Assert(uDelIndex < (UInt32)m_nPosition);
        if (uDelIndex >= (UInt32)m_nPosition)
        {
            UNLOCK_L(m_Lock);
            return;
        }

        //------------------------------------------------------------//
        //|   ...   |  ...      |        uDelIndex     |     ...       |      m_nPosition-1  |     m_nPosition (待分配池 ) |

        //|   ...   |  ...      |      m_nPosition -1 | ...            |   m_nPosition(待飞配池) |

        m_nPosition--;
        T *pDelObj = m_papObj[uDelIndex];
        m_papObj[uDelIndex] = m_papObj[m_nPosition];
        m_papObj[m_nPosition] = pDelObj;

        m_papObj[uDelIndex]->SetPoolID(uDelIndex);
        m_papObj[m_nPosition]->SetPoolID(-1);

        mFreeMap[pObj] = 1;

        pObj->cleanUp();

        UNLOCK_L(m_Lock);
        return;
    }

    Int32 GetCount(void)const {
        return m_nPosition;
    }

private:
    T                **m_papObj = nullptr;
    Int32            m_nMaxCount = 0;
    Int32            m_nPosition = 0;
    std::mutex        m_Lock;
};

#endif
