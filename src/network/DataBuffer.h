#ifndef _DATA_BUFFER_H_
#define _DATA_BUFFER_H_
#include "IBufferHandler.h"
#include "PacketHeader.h"
#include <cassert>
template <int SIZE>
class  CBufferManager;



template <int SIZE>
class CDataBuffer : public IDataBuffer
{
public:
    CDataBuffer(void)
    {
        m_nDataLen      = 0;
        m_nBufSize      = SIZE;
        m_nRefCount    = 0;
        m_pPrev         = nullptr;
        m_pNext         = nullptr;
        m_pManager      = nullptr;
    }

    virtual ~CDataBuffer(void)
    {
        m_nDataLen = 0;
        m_nBufSize = SIZE;
        m_nRefCount = 0;
        m_pPrev = nullptr;
        m_pNext = nullptr;
        m_pManager = nullptr;
    }

    bool AddRef()
    {
        m_pManager->m_BuffMutex.lock();
        m_nRefCount++;
        m_pManager->m_BuffMutex.unlock();
        return true;
    }

    bool Release()
    {
        assert(m_pManager != nullptr);

        m_pManager->ReleaseDataBuff(this);

        return true;
    }

    char* GetData()
    {
        return m_Buffer + HEADER_LEN;
    }

    int32_t GetTotalLenth()
    {
        return m_nDataLen;
    }

    int32_t GetBodyLenth()
    {
        return m_nDataLen - HEADER_LEN;
    }

    void SetTotalLenth(int32_t nPos)
    {
        m_nDataLen = nPos;
    }

    char* GetBuffer()
    {
        return m_Buffer;
    }

    int32_t GetBufferSize()
    {
        return m_nBufSize;
    }

    int32_t  CopyFrom(IDataBuffer* pSrcBuffer)
    {
        memcpy(m_Buffer, pSrcBuffer->GetBuffer(), pSrcBuffer->GetTotalLenth());

        m_nDataLen = pSrcBuffer->GetTotalLenth();

        return m_nDataLen;
    }

    int32_t  CopyTo(char* pDestBuf, int32_t nDestLen)
    {
        if(nDestLen < GetTotalLenth())
        {
            return 0;
        }

        memcpy(pDestBuf, GetBuffer(), GetTotalLenth());

        return nDestLen;
    }

    CDataBuffer<SIZE>* m_pPrev;

    CDataBuffer<SIZE>* m_pNext;

    CBufferManager<SIZE>* m_pManager;

public:
    int32_t       m_nRefCount;

    int32_t       m_nBufSize;

    char        m_Buffer[SIZE];

    int32_t       m_nDataLen;
};

template <int SIZE>
class  CBufferManager
{
public:
    CBufferManager()
    {
        m_pUsedList = nullptr;
        m_pFreeList = nullptr;
        m_nBufferCount = 0;
        m_EnablePool = true;
    }

    ~CBufferManager()
    {
        ReleaseAll();
    }

    IDataBuffer* AllocDataBuff()
    {
        m_BuffMutex.lock();
        CDataBuffer<SIZE>* pDataBuffer = nullptr;
        if(m_pFreeList == nullptr)
        {
            pDataBuffer = new CDataBuffer<SIZE>();
            pDataBuffer->m_pManager = this;
        }
        else
        {
            pDataBuffer = m_pFreeList;

            m_pFreeList = m_pFreeList->m_pNext;

            if(m_pFreeList != nullptr)
            {
                m_pFreeList->m_pPrev = nullptr;
            }

            pDataBuffer->m_pNext = nullptr;
            pDataBuffer->m_pPrev = nullptr;
        }

        assert(pDataBuffer->m_nRefCount == 0);

        pDataBuffer->m_nRefCount = 1;

        if(m_pUsedList == nullptr)
        {
            m_pUsedList = pDataBuffer;
        }
        else
        {
            pDataBuffer->m_pNext = m_pUsedList;
            m_pUsedList->m_pPrev = pDataBuffer;
            pDataBuffer->m_pPrev = nullptr;
            m_pUsedList = pDataBuffer;
        }

        m_nBufferCount += 1;
        m_BuffMutex.unlock();
        return pDataBuffer;
    }

    bool ReleaseDataBuff(CDataBuffer<SIZE>* pBuff)
    {
        assert(pBuff != nullptr);
        if (pBuff == nullptr)
        {
            return false;
        }

        assert(pBuff->m_nRefCount > 0);
        if (pBuff->m_nRefCount <= 0)
        {
            return false;
        }
        std::lock_guard<std::mutex> lock(m_BuffMutex);
        pBuff->m_nRefCount--;

        if (pBuff->m_nRefCount <= 0)
        {
            pBuff->m_nDataLen = 0;
            //首先从己用中删除
            if (m_pUsedList == pBuff)
            {
                //自己是首结点
                m_pUsedList = pBuff->m_pNext;
                if (m_pUsedList != nullptr)
                {
                    m_pUsedList->m_pPrev = nullptr;
                }
            }
            else
            {
                assert(pBuff->m_pPrev != nullptr);
                pBuff->m_pPrev->m_pNext = pBuff->m_pNext;
                if (pBuff->m_pNext != nullptr)
                {
                    pBuff->m_pNext->m_pPrev = pBuff->m_pPrev;
                }
            }

            if (m_EnablePool)
            {
                //再把自己加到己用中
                pBuff->m_pNext = m_pFreeList;
                pBuff->m_pPrev = nullptr;
                m_pFreeList = pBuff;

                if (pBuff->m_pNext != nullptr)
                {
                    pBuff->m_pNext->m_pPrev = pBuff;
                }
            }
            else
            {
                delete pBuff;
            }
            m_nBufferCount--;
        }
        return true;
    }

    void ReleaseAll()
    {
        CDataBuffer<SIZE>* pBufferNode = m_pFreeList;
        while (pBufferNode)
        {
            CDataBuffer<SIZE>* pTempNode = pBufferNode;
            pBufferNode = pTempNode->m_pNext;
            delete pTempNode;
        }

        pBufferNode = m_pUsedList;
        while (pBufferNode)
        {
            CDataBuffer<SIZE>* pTempNode = pBufferNode;
            pBufferNode = pTempNode->m_pNext;
            delete pTempNode;
        }

        return;
    }

    void SetEnablePool(bool bEnablePool)
    {
        m_EnablePool = bEnablePool;
    }

    void PrintOutList(CDataBuffer<SIZE>* pList)
    {
        int32_t nCount = 0;
        CDataBuffer<SIZE>* pBufferNode = pList;
        if(pBufferNode == nullptr)
        {
            return ;
        }

        bool bNext = true;
        while(pBufferNode)
        {
            if(bNext)
            {
                nCount++;
                if(pBufferNode->m_pNext != nullptr)
                {
                    pBufferNode = pBufferNode->m_pNext;
                }
                else
                {
                    bNext = false;
                    pBufferNode = pBufferNode->m_pPrev;
                }
            }
            else
            {
                nCount++;
                pBufferNode = pBufferNode->m_pPrev;
            }
        }

        return ;
    }

    CDataBuffer<SIZE>* m_pFreeList;

    CDataBuffer<SIZE>* m_pUsedList;

    std::mutex  m_BuffMutex;

    int32_t       m_nBufferCount;

    bool        m_EnablePool;
private:
};

class CBufferAllocator
{
    CBufferAllocator();
    ~CBufferAllocator();
public:
    static CBufferAllocator* GetInstancePtr();

public:
    IDataBuffer* AllocDataBuff(int nSize);

    CBufferManager<64>     m_BufferManager64B;      //管理<=64B的内存池，
    CBufferManager<128>    m_BufferManager128B;     //管理<=128B的内存池，
    CBufferManager<256>    m_BufferManager256B;     //管理<=256B的内存池，
    CBufferManager<512>    m_BufferManager512B;     //管理<=512B的内存池，
    CBufferManager<1024>   m_BufferManager1K;       //管理<=1k的内存池，
    CBufferManager<2048>   m_BufferManager2K;       //管理<=2k的内存池，
    CBufferManager<4096>   m_BufferManager4K;       //管理<=4k的内存池，
    CBufferManager<8192>   m_BufferManager8K;       //管理<=8k的内存池，
    CBufferManager<16384>  m_BufferManager16K;      //管理<=16k的内存池，
    CBufferManager<32768>  m_BufferManager32K;      //管理<=32k的内存池，
    CBufferManager<65536>  m_BufferManager64K;      //管理<=64k的内存池，

    CBufferManager<10 * 1024 * 1014> m_BufferManagerAny;        //管理<=10M的内存, 并不用池管理, 直接申请, 直接释放.
};

#endif