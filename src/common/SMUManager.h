#ifndef _SMU_MANAGER_H_
#define _SMU_MANAGER_H_

#include "ShareMemAO.h"
#include "xLog.h"


enum SMPOOL_TYPE
{
    SMPT_SHAREMEM,
};

//ShareMemory 单元池
template<typename T>
class SMUPool
{
public:
    SMUPool() {
        m_pRefObjPtr = NULL;
        m_hObj = NULL;
        m_nMaxSize = -1;
        m_nPosition = -1;
    }
    
    ~SMUPool() {
        if (m_pRefObjPtr) {
            delete m_pRefObjPtr;
            m_pRefObjPtr = nullptr;
        }
        if (m_hObj) {
            delete[]m_hObj;
            m_hObj = nullptr;
        }
    }

public:
    bool Init(UInt32 nMaxCount, SM_KEY key, SMPOOL_TYPE SMPT) {

        m_pRefObjPtr = new ShareMemAO();

        assert(m_pRefObjPtr);

        if (!m_pRefObjPtr) {
            return false;
        }

        //m_pRefObjPtr->m_CmdArg = CMD_MODE_LOADDUMP;// g_CmdArgv;

        bool ret = m_pRefObjPtr->Attach(key, sizeof(T)*nMaxCount + sizeof(SMHead));

        if (SMPT == SMPT_SHAREMEM) {
            if (!ret) {
                ret = m_pRefObjPtr->Create(key, sizeof(T)*nMaxCount + sizeof(SMHead));
            }
        } else {
            if (!ret) {
                return false;
            }
        }

        if (!ret) {
//             if (m_pRefObjPtr->m_CmdArg == CMD_MODE_CLEARALL) {
//                 return true;
//             }

            XLOG(SHMEM_LOG_PATH, "Setup SMU block fail!");
            assert(ret);
            return ret;

        }

        m_nMaxSize = nMaxCount;
        m_nPosition = 0;
        m_hObj = new T*[m_nMaxSize];

        Int32 i;
        for (i = 0; i < m_nMaxSize; i++) {
            m_hObj[i] = reinterpret_cast<T*>(m_pRefObjPtr->GetTypePtr(sizeof(T), i));
            if (m_hObj[i] == NULL) {
                assert(m_hObj[i] != NULL);
                return false;
            }
        }

        m_key = key;

        return true;
    }

    bool Finalize() {

        assert(m_pRefObjPtr);
        m_pRefObjPtr->Destory();

        return true;
    }

    T* NewObj(void) {
        assert(m_nPosition < m_nMaxSize);

        if (m_nPosition >= m_nMaxSize) {
            return NULL;
        }

        T *pObj = m_hObj[m_nPosition];
        pObj->SetPoolID((UInt32)m_nPosition);
        m_nPosition++;

        return pObj;
    }

    void DeleteObj(T *pObj) {        
        assert(pObj != NULL);

        if (pObj == NULL) {
            return;
        }

        assert(m_nPosition > 0);
        if (m_nPosition <= 0) {
            return;
        }

        UInt32 uDelIndex = pObj->GetPoolID();
        assert(uDelIndex < (UInt32)m_nPosition);
        if (uDelIndex >= (UInt32)m_nPosition) {
            return;
        }

        m_nPosition--;
        T *pDelObj = m_hObj[uDelIndex];
        m_hObj[uDelIndex] = m_hObj[m_nPosition];
        m_hObj[m_nPosition] = pDelObj;

        m_hObj[uDelIndex]->SetPoolID(uDelIndex);
        m_hObj[m_nPosition]->SetPoolID(INVALID_ID);
    }

    T* GetPoolObj(Int32 iIndex) {
        assert(iIndex < m_nMaxSize);
        return m_hObj[iIndex];
    }

    Int32 GetPoolMaxSize() {
        return m_nMaxSize;
    }

    Int32 GetPoolSize() {
        return m_nPosition;
    }

    SM_KEY GetKey() {
        return m_key;
    }

    bool DumpToFile(char* FilePath) {
        if (!m_pRefObjPtr) {
            assert(m_pRefObjPtr);
            return false;
        }

        return m_pRefObjPtr->DumpToFile(FilePath);
    }

    bool MergeFromFile(char* FilePath) {
        if (!m_pRefObjPtr) {
            assert(m_pRefObjPtr);
            return false;
        }
        return m_pRefObjPtr->MergeFromFile(FilePath);
    }

    UInt32 GetHeadVer() {
        assert(m_pRefObjPtr);
        return m_pRefObjPtr->GetHeadVer();
    }

    void SetHeadVer(UInt32 ver) {
        assert(m_pRefObjPtr);
        return m_pRefObjPtr->SetHeadVer(ver);
    }

private:
    T                **m_hObj;                //管理对象SMU数组
    Int32            m_nMaxSize = 0;            //最大容量
    Int32            m_nPosition = 0;        //当前使用偏移
    ShareMemAO*        m_pRefObjPtr = nullptr;    //引用SMObject
    SM_KEY            m_key;                    //对应的ShareMemory Key
};

#endif