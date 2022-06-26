#pragma once
#include "xDefine.h"
#include "IBufferHandler.h"
#include "KcpServer.h"
#include "xSingleton.h"
#include "common.h"

class CellApp: public IPacketDispatcher,
              public xSingleton<CellApp>
{
public:
    int Init(Int32 nServerID);

    bool Uninit();

    bool Run();

    bool ConnectToCellMgr();

    bool RegisterToCellMgr();

    Int32 GetServerID() { return m_nServerID;}

    bool FixFrameNum(Int32 nFrames);
    
     // overide from IPacketDIspatcher
     bool DispatchPacket( NetPacket* pNetPacket) override;
    
     bool OnCloseConnect(uint32_t nConnID) override;
    
     bool OnNewConnect(uint32_t nConnID) override;
    
     bool OnSecondTimer() override;
protected:
    Int32 m_nServerID = 0;
    // 场景管理器
    //CSceneManager       m_SceneManager;
    KcpServer             m_KcpServer;
    COMPONENT_STATE       m_state = COMPONENT_STATE_INIT;

    //以下用于统计
    UInt64                 m_uLastTick = 0;
    Int32                  m_nRecvNum = 0;
    Int32                  m_nSendNum = 0;
    Int32                  m_nFps = 0;
    Int32                  m_nLastMsgID = 0;
    
public:
    //*****************消息处理定义开始*****************
    bool OnKcpMsgCWork(NetPacket *pNetPacket);
    bool OnKcpMsgSWork(NetPacket *pNetPacket);
    bool OnTcpMsgCellMgr(NetPacket *pNetPacket);
    //*****************消息处理定义结束*****************
};
