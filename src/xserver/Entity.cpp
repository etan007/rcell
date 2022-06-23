#include "Entity.h"
#include "EntityMgr.h"

//通用的消息分发点
bool EntityDispatcher::OnDispatchMsg(EntityMailBox& sender_mb, int nCmdId, int nCmdParam, const std::string& sData)
{
    auto nCmdType = nCmdId * CMD_ID_PRIFX_ + nCmdParam;
    auto pHandler = GetHandler(nCmdType);
    if (!pHandler)
    {
        XERR("EntityDispatcher::OnDispatchMsg, no hanlder! cmd:%d,param:%d", nCmdType);
        return false;
    }
    ::google::protobuf::Message *pMsg = pHandler->m_pMsg;
    if (!pMsg || !pMsg->ParseFromString(sData)) {
        XERR("EntityDispatcher::OnDispatchMsg! ParseFromString Error!, data size=%d", sData.size());
        return false;
    }
    XLOG("EntityDispatcher::OnDispatchMsg Succ!eId:%llu, cmd:%d,param:%d,data size=%d", sender_mb.m_eId, nCmdId, nCmdParam, sData.size());

    return pHandler->m_handler(sender_mb, pMsg);
}


Entity::Entity()
{
    m_mb.m_eId = LocalIDPool::getMe().GetNextId();
    // todo sky
    //m_mb.m_nServerId = SServerStartConfig::getMe().GetServerSGUID();
    EntityMgr::getMe().AddEntity(this);
	XLOG("Entity! eId:%d!", GetEId());
}
Entity::~Entity()
{
	XLOG("~Entity! eId:%d!", GetEId());
    EntityMgr::getMe().RemoveEntity(GetEId());

    if (m_uSaveDbTimerId > 0)
    {
       // CHeapTimer::Instance().DelTimer(m_uSaveDbTimerId);
        m_uSaveDbTimerId = 0;
    }
}


void Entity::OnRegistered(const std::string& sEntityName, bool bSucc)
{
    XLOG("Entity::OnRegistered,sEntityName=%s, %s!", sEntityName.c_str(), bSucc ? "Succ!" : "Failed!");
}


//通用的发送给客户端的消息分发点
bool Entity::SendToClient(EntityMailBox & sender_mb, int nCmdId, int nCmdParam, const std::string& sData) {
    XERR("Entity::SendToClient! not handle!nCmdId:%d,nCmdParam:%d,size:%d", nCmdId, nCmdParam, sData.size());
    return false;
}


//通用的消息分发点
bool Entity::OnDispatchMsg(EntityMailBox & sender_mb, int nCmdId, int nCmdParam, const std::string& sData, bool bSend2Client)
{
    if (bSend2Client)
    {
        if (!IsClientEntity())
        {
            XERR("Entity::OnDispatchMsg ERROR! entity no have client! entity_type=%d", GetEntityType());
            return false;
        }
		XLOG("Entity::OnDispatchMsg! SendToClient!eId:%llu entity_type=%d",GetEId(), GetEntityType());
        //直接分发给客户端
        return SendToClient(sender_mb, nCmdId, nCmdParam, sData);
    }
    else
    {
		XLOG("Entity::OnDispatchMsg 22 !eId:%llu entity_type=%d", GetEId(), GetEntityType());
        return m_eDispatcher.OnDispatchMsg(sender_mb, nCmdId, nCmdParam, sData);
    }
}


