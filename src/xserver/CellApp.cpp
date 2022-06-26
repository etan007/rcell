#include "CellApp.h"
#include "XLog.h"
#include "ServerConfig.h"
int CellApp::Init(Int32 nServerID)
{
    // 1.服务器配置
    ServerConfig config;
    if (config.load("server.xml"))
    {
        XERR("CellApp::Init load server.xml error!!!");
        return -1;
    }
        
    // 2.服务器节点配置
    if (!config.gameAddr(nServerID))
    {
        XERR("CellApp::Init  gameAddr serverid=%d,error!!!",nServerID);
        return -2;
    }

    // 3.初始化场景管理器
    /*if(!m_SceneManager.Init(true))
    {
        XERR("启动场景管理器失败!");
        return -3;
    }*/

    // 4.启动网络监听
    m_nServerID = nServerID;
    if(!m_KcpServer.start(config.gameAddr(nServerID)->ip,config.gameAddr(nServerID)->port,this))
    {
        XERR("CellApp::Init  startserver ip=%s,port=%d error!!!",config.gameAddr(nServerID)->ip.c_str(),config.gameAddr(nServerID)->port);
        return -4;
    }
    m_uLastTick = 0;
    m_nRecvNum = 0;
    m_nFps = 0;
    m_nSendNum = 0;
    m_nLastMsgID = 0;   
    m_state = COMPONENT_STATE_RUN;
    return 0;
}

bool CellApp::Uninit()
{
    return true;
}

bool CellApp::Run()
{
    while(m_state == COMPONENT_STATE_RUN)
    {
        m_KcpServer.runOneStep();
        //m_SceneManager.OnUpdate(CommonFunc::GetTickCount());
        m_nFps += 1;

        if((CommonFunc::GetTickCount() - m_uLastTick) > 1000)
        {
            m_nFps = 0;
            m_nRecvNum = 0;
            m_nSendNum = 0;
            m_uLastTick = CommonFunc::GetTickCount();
        }
        
        FixFrameNum(200);   //由于只是转发,尽可能快
    }
    return true;
}

bool CellApp::ConnectToCellMgr()
{
    return true;
}

bool CellApp::RegisterToCellMgr()
{
    return true;
}

bool CellApp::FixFrameNum(Int32 nFrames)
{
    if (nFrames < 1)
    {
        nFrames = 1;
    }

    static UInt64 uNextTick = CommonFunc::GetTickCount();
    UInt64 uCurTick = CommonFunc::GetTickCount();

    if (uNextTick > uCurTick)
    {
        CommonFunc::Sleep(uNextTick - uCurTick);
    }

    uNextTick = uNextTick + 1000 / nFrames;
    return true;
}

bool CellApp::DispatchPacket(NetPacket* pNetPacket)
{
    
    return true;
}

bool CellApp::OnCloseConnect(uint32_t nConnID)
{
    return true;
}

bool CellApp::OnNewConnect(uint32_t nConnID)
{
    return true;
}

bool CellApp::OnSecondTimer()
{
    return true;
}

bool CellApp::OnKcpMsgCWork(NetPacket* pNetPacket)
{
    return true;
}

bool CellApp::OnKcpMsgSWork(NetPacket* pNetPacket)
{
    return true;
}

bool CellApp::OnTcpMsgCellMgr(NetPacket* pNetPacket)
{
    return true;
}
