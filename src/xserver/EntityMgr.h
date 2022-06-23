#pragma once
#include "xSingleton.h"
#include <unordered_map>
#include <memory>
//#include "SystemInternalCmd.pb.h"
//#include "xCommand.h"
#include "xServerDispatcher.h"
#include <mutex>

class Entity;
typedef std::shared_ptr<class EntityMailBox> EntityMailBoxPtr;
class PBLobbyGateAddGlobalEntity;
class PBLobbyGateForwardMsg2Entity;
 
class EntityMgr : public xSingleton<EntityMgr>
{
public:

    void AddEntity(Entity* pEntity);
    void RemoveEntity(uint64_t eId);
    Entity* GetEntity(uint64_t eId);
    void UpdateGlobalEntity(PBLobbyGateAddGlobalEntity* pCmd);
    EntityMailBoxPtr GetGlobalEntity(const std::string& sGlobalEntityName);

    //�ڱ��ش���һ��ʵ��
    static Entity* CreateEntity(const std::string& strTypeName, const std::string* pData);
    static Entity * CreateEntity(const std::string & strTypeName);
    //��Զ�̴���һ��ʵ�壨LobbyCenter���и��ؾ��⣩
    static void CreateEntityAnyWhere(const std::string& strTypeName);
    template<class TEntityDataType>
    static void CreateEntityAnyWhereWithData(const std::string& strTypeName, TEntityDataType* data);
    static void CreateEntityAnyWhereWithData(const std::string& strTypeName, const char* pData, uint32_t nLen);
    //��Զ��ע��һ��ȫ��ʵ�壨��ͬ����ʵ�壬ֻ���һ��ע��ĳɹ���
    static void RegisterGlobalEntity(const std::string & sEntityName, Entity * pEntity);

    bool OnDispatchMsg(PBLobbyGateForwardMsg2Entity *pMsg);

private:
	std::mutex							m_entitiesMutex;
    std::unordered_map<uint64_t, Entity*> m_entities;
    //ȫ�ֹ���ʵ��
	std::mutex							m_globalEntitiesMutex;
    std::unordered_map<std::string, EntityMailBoxPtr> m_globalEntities;
};


template<class TEntityDataType>
void EntityMgr::CreateEntityAnyWhereWithData(const std::string & sEntityName, TEntityDataType* data)
{
    /*
    XLOG("CreateEntityAnyWhereWithData T! strTypeName:%s", sEntityName.c_str());
    PBLobbyGateCreateEntityAnyWhere msg;
    msg.set_entity_name(sEntityName);
    if (data)
        data->SerializeToString(msg.mutable_entity_data());

    SGUID_UINT32 tempSGUID;
    tempSGUID.setSid(SERVER_SID_LOBBY_CENTER);
    tempSGUID.setGid(1);
    tempSGUID.setNid(0);
    xServerDispatcher::SendToOtherServer(tempSGUID.getSGUID(), msg);*/
}

