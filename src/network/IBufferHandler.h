#ifndef __IBUFFER_HANDLER_H__
#define __IBUFFER_HANDLER_H__
#include "xDefine.h"
struct NetPacket;

class IDataBuffer
{
public:
    virtual bool    AddRef() = 0;

    virtual bool    Release() = 0;

    virtual char*   GetData() = 0;

    virtual int32_t   GetTotalLenth() = 0;

    virtual void    SetTotalLenth(int32_t nPos) = 0;

    virtual int32_t   GetBodyLenth() = 0;

    virtual char*   GetBuffer() = 0;

    virtual int32_t   GetBufferSize() = 0;

    virtual int32_t   CopyFrom(IDataBuffer* pSrcBuffer) = 0;

    virtual int32_t   CopyTo(char* pDestBuf, int32_t nDestLen) = 0;
};

struct IDataHandler
{
    virtual bool OnDataHandle( IDataBuffer* pDataBuffer, uint32_t nConnID) = 0;
    virtual bool OnCloseConnect(uint32_t nConnID) = 0;
    virtual bool OnNewConnect(uint32_t nConnID) = 0;
};

struct IPacketDispatcher
{
    virtual bool DispatchPacket( NetPacket* pNetPacket) = 0;
    virtual bool OnCloseConnect(uint32_t nConnID) = 0;
    virtual bool OnNewConnect(uint32_t nConnID) = 0;
    virtual bool OnSecondTimer() = 0;
};

struct NetPacket
{
    NetPacket(uint32_t nConnID = 0, IDataBuffer* pBuffer = nullptr, int32_t nMsgID = 0 )
    {
        m_nConnID = nConnID;

        m_pDataBuffer = pBuffer;

        m_nMsgID = nMsgID;
    }

    int32_t      m_nMsgID;
    uint32_t     m_nConnID;
    IDataBuffer* m_pDataBuffer;
};




#endif /* __IBUFFER_HANDLER_H__ */
