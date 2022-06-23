#pragma once
#include <string>
#include "xSingleton.h"
//#include "SystemInternalCmd.pb.h"
#include <unordered_map>
#include "EntityMgr.h"
#include "EntityMailbox.h"
//#include "ServerClient.h"
#include "xServerDispatcher.h"
#include "balance.h"

class EntityRegisterMgr : public xSingleton<EntityRegisterMgr>
{
public:

    void Init();
public:
   /* void RegisterGlobalEntityReq(PBLobbyGateRegisterGlobalEntityReq* pCmd);
    void RegisterGlobalEntityRsp(PBLobbyGateRegisterGlobalEntityRsp *pCmd);
    bool CreateEntityAnyWhereReq(PBLobbyGateCreateEntityAnyWhere *pCmd);
    bool CreateEntityFromCenterReq(PBLobbyGateCreateEntityFromCenter *pCmd);

    bool OnServerConnected(xNetProcessor * pNetProcess);
    bool AllServerConnected(PBAllServerConnected *pCmd);*/
private:
    std::unordered_map<std::string, EntityMailBox> m_globalEntities;     //全局唯一的公共实体
    CBalance m_lobbyBalance;


    std::unordered_map<uint32_t, uint32_t> m_connectedServerMap;
};

