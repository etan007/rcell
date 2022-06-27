#pragma once
#include <string>
#include <functional>
#include <unordered_map>

#include "google/protobuf/message.h"
#include "xLog.h"
//#include "SStartConfig.h"
#include "xMsgDispatcher.h"
#include "EntityMailbox.h" 
#include "Entity.pb.h"
#include "LocalIDPool.h"
#include "SystemInternalCmd.pb.h"
#include "xServerDispatcher.h"
#include "EntityFactory.h"
#include "EntityMgr.h"
#include "CommonCmd.pb.h"

using namespace std::placeholders;
 
//公共实体类型
enum enEntityType
{
    PET_DEFAULT = 0,		 //本地默认实体
    PET_CLIENT  = 1,         //客户端实体
    PET_DS      = 2,         //DS实体
    PET_GLOBAL_ENTITY = 2   //全局实体
};

typedef std::function<bool(EntityMailBox& mb, ::google::protobuf::Message *pMsg)> OnEntityPBCallBack;


class EntityDispatcher
{
    struct EntityPBHandler
    {
        ::google::protobuf::Message *m_pMsg;
        OnEntityPBCallBack m_handler;
    };
public:
    //注册消息处理函数
    template<class TMsg>
    void RegHandler(OnEntityPBCallBack callBack)
    {
        auto msg = new TMsg();
        EntityPBHandler handler;
        handler.m_handler = callBack;
        handler.m_pMsg = msg;
        m_pbHandler[msg->cmdid().cmd() * CMD_ID_PRIFX_ + msg->param()] = handler;
    }

    EntityPBHandler* GetHandler(uint32_t nCmdType)
    {
        auto iter = m_pbHandler.find(nCmdType);
        if (iter == m_pbHandler.end())
            return nullptr;
        return &iter->second;
    }

    template<class TMsg>
    bool OnDispatchMsg(EntityMailBox & sender_mb, const TMsg& msg)
    {
        auto nCmdType = msg.cmdid().cmd() * CMD_ID_PRIFX_ + msg.param();
        auto pHandler = GetHandler(nCmdType);
        if (!pHandler)
        {
            XERR("EntityDispatcher::OnDispatchMsg, no hanlder! cmd:%d,param:%d", nCmdType);
            return false;
        }
        return pHandler->m_handler(sender_mb, (::google::protobuf::Message*)&msg);
    }


    bool OnDispatchMsg(EntityMailBox & sender_mb, int nCmdId, int nCmdParam, const std::string& sData);
private:
    std::unordered_map<uint32_t, EntityPBHandler> m_pbHandler;
};


//通用实体
class Entity : public std::enable_shared_from_this<Entity>
{
public:
    Entity();
    virtual ~Entity();


public:
    //注册返回函数
    virtual void OnRegistered(const std::string& sEntityName, bool bSucc);
    //通用的发送给客户端的消息分发点
    virtual bool SendToClient(EntityMailBox & sender_mb, int nCmdId, int nCmdParam, const std::string& sData);
    //通用的消息分发点
    bool OnDispatchMsg(EntityMailBox & sender_mb, int nCmdId, int nCmdParam, const std::string& sData, bool bSend2Client);

    virtual void OnSave2Db() {
        XERR("OnSave2Db! save nothing! eId = %llu", m_mb.m_eId);
    }

    virtual void OnTimerRefresh()
    {

    }

    bool StartSaveToDb()
    {
  
        return true;
    }
	bool StartTimerRefresh()
	{
 
		return true;
	}

public:
    //注册消息处理函数
    template<class TMsg>
    void RegHandler(OnEntityPBCallBack callBack);

    //给实体发送消息
    template<class TMsg>
    bool SendToEntity(const EntityMailBox& targetMb, const TMsg& msg);

    //给客户端实体发送消息（可以直接通过该玩家所在lobby转发给玩家客户端）
    template<class TMsg>
    bool SendToClientEntity(const EntityMailBox& targetMb, const TMsg& msg);

    //给全局实体发送消息
    template<class TMsg>
    bool SendToGlobalEntity(const std::string& sGlobalEntityName, const TMsg& msg);

public:
    void CopyMbTo(PBEntityMailbox* pPBMailbox)
    {
        m_mb.CopyTo(pPBMailbox);
    }

    uint64_t GetEId() { return m_mb.m_eId; }
    uint32_t GetServerId() { return m_mb.m_nServerId; }
    enEntityType GetEntityType() { return m_eType; }
    void SetEntityType(enEntityType eType) { m_eType = eType; }
    EntityMailBox& GetEntityMb() { return m_mb; }
    bool IsClientEntity() { return (GetEntityType() == PET_CLIENT); }   //是否客户端实体

private:
    //给实体发送消息
    template<class TMsg>
    bool SendMsgTo(const EntityMailBox& targetMb, const TMsg& msg, bool send2Client);

    //本地分发用
    template<class TMsg>
    bool OnLocalDispatchMsg(EntityMailBox & sender_mb, const TMsg& msg, bool bSend2Client);


protected:
    enEntityType        m_eType = PET_DEFAULT;             //实体类型
    EntityMailBox       m_mb;                           //实体邮箱
    EntityDispatcher    m_eDispatcher;                  //消息分发器

    uint64_t            m_uSaveDbTimerId = 0;
    uint64_t            m_uRefreshTimerId = 0;
};

typedef std::shared_ptr<Entity> EntityPtr;


//注册消息处理函数
template<class TMsg>
void Entity::RegHandler(OnEntityPBCallBack callBack)
{
    m_eDispatcher.RegHandler<TMsg>(callBack);
}

//给实体发送消息
template<class TMsg>
bool Entity::SendToEntity(const EntityMailBox& targetMb, const TMsg& msg)
{
    return SendMsgTo(targetMb, msg, false);
}

//给客户端实体发送消息（可以直接通过该玩家所在lobby转发给玩家客户端）
template<class TMsg>
bool Entity::SendToClientEntity(const EntityMailBox& targetMb, const TMsg& msg)
{
    return SendMsgTo(targetMb, msg, true);
}

//给全局实体发送消息
template<class TMsg>
bool Entity::SendToGlobalEntity(const std::string& sGlobalEntityName, const TMsg& msg)
{
    auto pGeMb = EntityMgr::getMe().GetGlobalEntity(sGlobalEntityName);
    if (!pGeMb)
    {
        XERR("SendToGlobalEntity! Not find entity:%s", sGlobalEntityName.c_str());
        return false;
    }
    return SendToEntity(*pGeMb, msg);
}


//给实体发送消息
template<class TMsg>
bool Entity::SendMsgTo(const EntityMailBox& targetMb, const TMsg& msg, bool send2Client)
{
    if (!targetMb.IsOk())
    {
        XERR("Entity::SendMsgTo error! mb is invaild! eid=%llu,server_id=%d", targetMb.m_eId, targetMb.m_nServerId);
        return false;
    }

    if (targetMb.IsLocalServer())
    {//如果实体在本进程内，直接查找并调用，不需要通过网络。
        auto pEntity = EntityMgr::getMe().GetEntity(targetMb.m_eId);
        if (!pEntity)
        {
            XERR("ForwardMsg! Not find entity:%d", targetMb.m_eId);
            return false;
        }

        return pEntity->OnLocalDispatchMsg(m_mb, msg, send2Client);
    }
    else
    {
        PBLobbyGateForwardMsg2Entity forwardMsg;
        forwardMsg.set_target_eid(targetMb.m_eId);
        forwardMsg.set_send2client(send2Client);
        m_mb.CopyTo(forwardMsg.mutable_sender_mb());
        forwardMsg.set_sub_cmdid(msg.cmdid().cmd());
        forwardMsg.set_sub_param(msg.param());
        msg.SerializeToString(forwardMsg.mutable_forward_data());
        return xServerDispatcher::SendToOtherServer(targetMb.m_nServerId, forwardMsg);
    }

}

//本地分发用
template<class TMsg>
bool Entity::OnLocalDispatchMsg(EntityMailBox & sender_mb, const TMsg& msg, bool bSend2Client)
{
    if (bSend2Client)
    {
        if (!IsClientEntity())
        {
            XERR("Entity::OnDispatchMsgT ERROR! entity no have client! entity_type=%d", GetEntityType());
            return false;
        }

        //直接分发给客户端
        string sData;
        msg.SerializeToString(&sData);
        return SendToClient(sender_mb, msg.cmdid().cmd(), msg.param(), sData);
    }
    else
    {
        return m_eDispatcher.OnDispatchMsg(sender_mb, msg);
    }
}



////////////////////////////////////////////////////////////////动态实体/////////////////////////////////////////////////////////


//可以动态创建的实体（通过反射机制）
template<class TType>
class TDynamicEntity : public DynamicCreator<TType>, public Entity
{
public:
    TDynamicEntity() {
        //处理通用的上下线消息
        RegHandler<PBLobbyGateUserOnlineReq>(std::bind(&TDynamicEntity<TType>::OnPBLobbyGateUserOnlineReq, this, _1, _2));    //玩家上线
        RegHandler<PBLobbyGateUserOfflineReq>(std::bind(&TDynamicEntity<TType>::OnPBLobbyGateUserOfflineReq, this, _1, _2));    //玩家下线
        RegHandler<PBPlayerBaseDataSimpleSyn>(std::bind(&TDynamicEntity<TType>::OnPBLobbyGateUserBaseDataSimpleSyn, this, _1, _2));    //玩家数据更新
    }
public:
    virtual void SetData(const std::string& data) 
    {
        m_bInitData = InitWithData(data);
        if (!m_bInitData)
        {
            XERR("Entity::SetData Failed! data_size:%d", data.size());
        }
    }

    virtual bool InitWithData(const std::string& data)
    {
        XERR("Entity::InitWithData! data_size:%d", data.size());
        return false;
    }
    /*
    virtual bool OnPlayerOnline(EntityMailBox& sender_mb, PBLobbyGateUserOnlineReq* pOnlineCmd)
    {
        XLOG("TGlobalEntity::OnPlayerOnline!entity_name:%s, uid:%d", this->GetTypeName().c_str(), pOnlineCmd->uid());
        return false;
    }

    virtual bool OnPlayerOffline(EntityMailBox& sender_mb, PBLobbyGateUserOfflineReq* pOfflineCmd)
    {
        XLOG("TGlobalEntity::OnPlayerOffline!entity_name:%s, uid:%d", this->GetTypeName().c_str(), pOfflineCmd->uid());
        return false;
    }
	virtual bool OnPlayerBaseDataSimpleSyn(EntityMailBox& sender_mb, PBPlayerBaseDataSimpleSyn* pBaseDataCmd)
	{
		XLOG("TGlobalEntity::OnPlayerBaseDataSimpleSyn!entity_name:%s, uid:%d", this->GetTypeName().c_str(), pBaseDataCmd->data().uid());
		return false;
	}*/
public:

    inline bool OnPBLobbyGateUserOnlineReq(EntityMailBox& sender_mb, ::google::protobuf::Message *pMsg)
    {
        //return OnPlayerOnline(sender_mb, (PBLobbyGateUserOnlineReq*)pMsg);
        return false;
    }

    inline bool OnPBLobbyGateUserOfflineReq(EntityMailBox& sender_mb, ::google::protobuf::Message *pMsg)
    {
       //return OnPlayerOffline(sender_mb, (PBLobbyGateUserOfflineReq*)pMsg);
        return false;
    }
	inline bool OnPBLobbyGateUserBaseDataSimpleSyn(EntityMailBox& sender_mb, ::google::protobuf::Message* pMsg)
	{
		//return OnPlayerBaseDataSimpleSyn(sender_mb, (PBPlayerBaseDataSimpleSyn*)pMsg);
        return false;
	}
private:
    bool  m_bInitData = false;
};


//客户端实体
template<class TType>
class TClientEntity : public TDynamicEntity<TType>
{
public:
    TClientEntity() { this->m_eType = PET_CLIENT; }
};

//DS实体
template<class TType>
class TDSEntity : public TDynamicEntity<TType>
{
public:
    TDSEntity() { this->m_eType = PET_DS; }
};

//全局实体
template<class TType>
class TGlobalEntity : public TDynamicEntity<TType>
{
public:
    TGlobalEntity() { 
        this->m_eType = PET_GLOBAL_ENTITY; 
		XLOG("TGlobalEntity::ctor!  eId:%llu,entity type_name:%s", this->GetEId(),this->GetTypeName().c_str());
        Entity::RegHandler<PBNotifyGlobalEntityStartInit>(std::bind(&TGlobalEntity<TType>::OnPBNotifyGlobalEntityStartInit, this, _1, _2));    //通知开始初始化
    }

    virtual void OnRegistered(const std::string & sEntityName, bool bSucc)
    {
        if (!bSucc)
        {
            XLOG("TGlobalEntity::OnRegistered! Failed!entity type_name:%s", this->GetTypeName().c_str());
            return;
        }
        XLOG("TGlobalEntity::OnRegistered! Successed! entity type_name:%s", this->GetTypeName().c_str());

        NotifyLoadSucc();
    }


    virtual bool OnInit()
    {
        XLOG("TGlobalEntity::OnInit! entity type_name:%s", this->GetTypeName().c_str());
        NotifyInitSucc();
        return true;
    }

public:

    inline bool OnPBNotifyGlobalEntityStartInit(EntityMailBox& sender_mb, ::google::protobuf::Message *pMsg)
    {
        return OnInit();
    }

    void NotifyLoadSucc()
    {
        m_bLoadSucc = true;
        PBMgrLoadedSuccessed loadSucc;
        loadSucc.set_entity_name(this->GetTypeName());
        this->SendToGlobalEntity("GameMgr", loadSucc);
    }

    void NotifyInitSucc()
    {
        m_bInitSucc = true;
        PBMgrInitedSuccessed initSucc;
        initSucc.set_entity_name(this->GetTypeName());
        this->SendToGlobalEntity("GameMgr", initSucc);
    }

protected:
    bool m_bInitSucc = false;
    bool m_bLoadSucc    = false;
};


