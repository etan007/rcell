#include "EntityMgr.h"
#include "Entity.h"
#include "EntityMailbox.h"
#include "EntityFactory.h"
#include "EntityRegisterMgr.h"

Entity* EntityMgr::CreateEntity(const std::string & strTypeName, const std::string* pData)
{
    return EntityFactory::getMe().CreateEntity(strTypeName, pData);
}

Entity* EntityMgr::CreateEntity(const std::string & strTypeName)
{
    return CreateEntity(strTypeName, nullptr);
}

void EntityMgr::CreateEntityAnyWhere(const std::string & strTypeName)
{
    CreateEntityAnyWhereWithData(strTypeName, (::google::protobuf::Message*)nullptr);
}

void EntityMgr::CreateEntityAnyWhereWithData(const std::string & strTypeName, const char * pData, uint32_t nLen)
{
    XLOG("CreateEntityAnyWhereWithData! strTypeName:%s", strTypeName.c_str());
   /* PBLobbyGateCreateEntityAnyWhere msg;
    msg.set_entity_name(strTypeName);
    msg.set_entity_data(pData, nLen);
    SGUID_UINT32 tempSGUID;
    tempSGUID.setSid(SERVER_SID_LOBBY_CENTER);
    tempSGUID.setGid(1);
    tempSGUID.setNid(0);
    xServerDispatcher::SendToOtherServer(tempSGUID.getSGUID(), msg);*/

}

//向LobbyCenter注册某个实体
void EntityMgr::RegisterGlobalEntity(const std::string& sEntityName, Entity* pEntity)
{
    if (!EntityMgr::getMe().GetEntity(pEntity->GetEId()))
    {
        XERR("entity not find in EntityMgr! eid=%d", pEntity->GetEId());
        return;
    }

    if (PET_GLOBAL_ENTITY != pEntity->GetEntityType())
    {
        assert(false);
        XERR("entity not global entity! eid=%d", pEntity->GetEId());
        return;
    }

    XLOG("EntityMgr::RegisterGlobalEntity sEntityName=%s", sEntityName.c_str());
    /*
    PBLobbyGateRegisterGlobalEntityReq registerMsg;
    registerMsg.set_entity_name(sEntityName);
    pEntity->CopyMbTo(registerMsg.mutable_entity_mb());

    SGUID_UINT32 tempSGUID;
    tempSGUID.setSid(SERVER_SID_LOBBY_CENTER);
    tempSGUID.setGid(1);
    tempSGUID.setNid(0);
    xServerDispatcher::SendToOtherServer(tempSGUID.getSGUID(), registerMsg);*/
}



void EntityMgr::AddEntity(Entity* pEntity)
{
	std::lock_guard<std::mutex> lock_guard(m_entitiesMutex);
    m_entities[pEntity->GetEId()] = pEntity;
}
void EntityMgr::RemoveEntity(uint64_t eId)
{
	std::lock_guard<std::mutex> lock_guard(m_entitiesMutex);
    m_entities.erase(eId);
}

Entity* EntityMgr::GetEntity(uint64_t eId)
{
	std::lock_guard<std::mutex> lock_guard(m_entitiesMutex);
    auto iter = m_entities.find(eId);
    if (iter == m_entities.end())
        return nullptr;
    return iter->second;
}

void EntityMgr::UpdateGlobalEntity(PBLobbyGateAddGlobalEntity* pCmd)
{
    /*
    XLOG("EntityMgr::UpdateGlobalEntity !Start!=========================>");

	std::lock_guard<std::mutex> lock_guard(m_globalEntitiesMutex);
    for (auto i = 0; i < pCmd->global_entities_size(); ++i)
    {
        auto& global_entity = pCmd->global_entities(i);
        EntityMailBoxPtr ge = std::make_shared<EntityMailBox>();
        ge->CopyFrom(global_entity.entity_mb());
        m_globalEntities[global_entity.entity_name()] = ge;

        XLOG("EntityMgr::UpdateGlobalEntity !entity_name=%s,entity_id=%llu,server_id=%d", global_entity.entity_name().c_str(), ge->m_eId, ge->m_nServerId);
    }
    XLOG("EntityMgr::UpdateGlobalEntity !End!<=========================");*/
}

EntityMailBoxPtr EntityMgr::GetGlobalEntity(const std::string& sGlobalEntityName)
{
	std::lock_guard<std::mutex> lock_guard(m_globalEntitiesMutex);
    auto iter = m_globalEntities.find(sGlobalEntityName);
    if (iter == m_globalEntities.end())
        return nullptr;
    return iter->second;
}


bool EntityMgr::OnDispatchMsg(PBLobbyGateForwardMsg2Entity *pCmd)
{
    /*
    auto pEntity = GetEntity(pCmd->target_eid());
    if (!pEntity)
    {
        XERR("EntityMgr::OnDispatchMsg ! not find entity:%d", pCmd->target_eid());
        return true;
    }

    EntityMailBox sender_mb(pCmd->sender_mb().entity_id(), pCmd->sender_mb().server_id());
    bool bRet = pEntity->OnDispatchMsg(sender_mb, pCmd->sub_cmdid(), pCmd->sub_param(), pCmd->forward_data(), pCmd->send2client());
    if (!bRet)
    {
        XERR("EntityMgr::OnDispatchMsg ! return false!!! target_eid:%d,cmdId=%d, param=%d, ", pCmd->target_eid(), pCmd->sub_cmdid(), pCmd->sub_param());
    }*/
    return true;
}
