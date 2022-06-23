#include "EntityRegisterMgr.h"
#include "Entity.h"
#include "EntityMgr.h"
 
void EntityRegisterMgr::Init()
{
    XLOG("EntityRegisterMgr::Init!");
}
/*
void EntityRegisterMgr::RegisterGlobalEntityReq(PBLobbyGateRegisterGlobalEntityReq* pCmd)
{

    auto pEntityMb = pCmd->mutable_entity_mb();
    uint32_t serverId = pEntityMb->server_id();
    uint64_t eId = pEntityMb->entity_id();
    const std::string& sEntityName = pCmd->entity_name();
    XLOG("RegisterGlobalEntity! serverId=%d,entity_name=%s", serverId, sEntityName.c_str());


    bool bSucc = false;
    auto iter = m_globalEntities.find(sEntityName);
    if (iter == m_globalEntities.end())
    {//没有注册过
        bSucc = true;
    }
    else if (iter->second.m_nServerId == serverId)
    {//同一个lobbygate多次注册，表示更新
        iter->second.m_eId = eId;
        bSucc = true;
    }


    if (bSucc)
    {
        XLOG("RegisterGlobalEntity Succ! serverId=%d,entity_name=%s", serverId, sEntityName.c_str());

        EntityMailBox ge;
        ge.m_nServerId = serverId;
        ge.m_eId = eId;
        m_globalEntities[sEntityName] = ge;

        if (m_lobbyBalance.IsHas(serverId))
        {//只添加lobbygate的权重。
            m_lobbyBalance.AddWeight(serverId, 1);
        }

        PBLobbyGateAddGlobalEntity addMsg;

        for (auto& v : m_globalEntities)
        {
            auto pGlobalEntity= addMsg.add_global_entities();
            pGlobalEntity->set_entity_name(v.first);
            v.second.CopyTo(pGlobalEntity->mutable_entity_mb());
        }

        //通知所有lobbyGate,有全局实体注册
        xServerDispatcher::BroadcastMsgToServer(SERVER_SID_LOBBY_GATE, addMsg);

        //通知mailServer,有全局实体注册
        xServerDispatcher::SendToOtherServer(SGUID_UINT32::GetSGUID(SERVER_SID_MAIL_SERVER,1,0) , addMsg);

        //通知GmServer,有全局实体注册
        xServerDispatcher::SendToOtherServer(SGUID_UINT32::GetSGUID(SERVER_SID_GM_SERVER, 1, 0), addMsg);

        //通知DataRecord,有全局实体注册
        xServerDispatcher::SendToOtherServer(SGUID_UINT32::GetSGUID(SERVER_SID_DATE_RECORD, 1, 0), addMsg);

        //通知Match,有全局实体注册
        xServerDispatcher::SendToOtherServer(SGUID_UINT32::GetSGUID(SERVER_SID_MATCH, 1, 0), addMsg);

		//通知TradeBank,有全局实体注册
		xServerDispatcher::SendToOtherServer(SGUID_UINT32::GetSGUID(SERVER_SID_TRADEBANK, 1, 0), addMsg);
    }

    PBLobbyGateRegisterGlobalEntityRsp rspMsg;
    rspMsg.mutable_entity_mb()->set_entity_id(eId);
    rspMsg.mutable_entity_mb()->set_server_id(serverId);
    rspMsg.set_entity_name(sEntityName);
    rspMsg.set_code(bSucc ? 0 : 1);
    xServerDispatcher::SendToOtherServer(serverId, rspMsg);
}

void EntityRegisterMgr::RegisterGlobalEntityRsp(PBLobbyGateRegisterGlobalEntityRsp *pCmd)
{
    auto pEntityMb = pCmd->mutable_entity_mb();
    auto& sEntityName = pCmd->entity_name();
    auto eId = pEntityMb->entity_id();
    bool bSucc = (0 == pCmd->code());

    auto pEntity = EntityMgr::getMe().GetEntity(eId);
    if (!pEntity)
    {
        XERR("RegisterGlobalEntityCallback not find entity! eid=%d", eId);
        return;
    }

    if (!bSucc)
    {
        EntityMgr::getMe().RemoveEntity(eId);
    }
    pEntity->OnRegistered(sEntityName, bSucc);
}

bool EntityRegisterMgr::CreateEntityAnyWhereReq(PBLobbyGateCreateEntityAnyWhere *pCmd)
{//请求LobbyCenter通过负载均衡在某个lobbygate上创建一个实体

    auto nMinLobbyGateId = m_lobbyBalance.GetLestWeightId();
    m_lobbyBalance.AddWeight(nMinLobbyGateId, 1);

    XLOG("EntityRegisterMgr::CreateEntityAnyWhereReq! strTypeName:%s,lobby_gate_id:%d", pCmd->entity_name().c_str(), nMinLobbyGateId);
    PBLobbyGateCreateEntityFromCenter createEntityMsg;
    createEntityMsg.set_entity_name(pCmd->entity_name());
    createEntityMsg.set_entity_data(pCmd->entity_data());
    xServerDispatcher::SendToOtherServer(nMinLobbyGateId, createEntityMsg);
    return true;
}

bool EntityRegisterMgr::CreateEntityFromCenterReq(PBLobbyGateCreateEntityFromCenter *pCmd)
{//lobbycenter通知某个lobbygate创建指定一个实体
    XLOG("EntityRegisterMgr::CreateEntityFromCenterReq! strTypeName:%s", pCmd->entity_name().c_str());
    EntityMgr::CreateEntity(pCmd->entity_name(),&pCmd->entity_data());
    return true;
}

bool EntityRegisterMgr::OnServerConnected(xNetProcessor * pNetProcess)
{
    //通知最新的实体信息
    PBLobbyGateAddGlobalEntity addMsg;
    for (auto& v : m_globalEntities)
    {
        auto pGlobalEntity = addMsg.add_global_entities();
        pGlobalEntity->set_entity_name(v.first);
        v.second.CopyTo(pGlobalEntity->mutable_entity_mb());
    }

    xServerDispatcher::SendToOtherServer(pNetProcess->m_u32SGUID, addMsg);
    return true;
}

bool EntityRegisterMgr::AllServerConnected(PBAllServerConnected * pCmd)
{

    auto nTotalCount = SServerStartConfig::getMe().GetEnableServerCount() - 2; //去掉自己和Mananger.exe两个进程。
    m_connectedServerMap[pCmd->sguid()] = 1;

    XLOG("EntityRegisterMgr::AllServerConnected! server id:%d!!!total=%d,now_count:%d", pCmd->sguid(), nTotalCount, m_connectedServerMap.size());
    if (m_connectedServerMap.size() == nTotalCount)
    {
        //初始化负载均衡器
        list<ServerClient *> outList;
        xServerDispatcher::GetNetConnectListSameAsSType(SERVER_SID_LOBBY_GATE, true, outList);

        for (auto &it : outList) {
            if (it->pNetProcess) {
                XLOG("EntityRegisterMgr::AddNewId! server id:%d!!!", it->pNetProcess->m_u32SGUID);
                m_lobbyBalance.AddNewId(it->pNetProcess->m_u32SGUID);
            }
        }

        XLOG("EntityRegisterMgr::AllServerConnected! total count:%d", nTotalCount);
        PBAllServerStartNow startMsg;
        startMsg.set_bstart(1);

        for (auto& v : m_connectedServerMap)
        {
            GetServer()->SendToOtherServer(v.first, startMsg);
        }
    }
    return true;
}*/
