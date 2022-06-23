#include "xMsgDispatcher.h"
//#include "xCommand.h"
//#include "SystemInternalCmd.pb.h"

#include "EntityMgr.h"
#include "EntityRegisterMgr.h"

void MsgDispatcher::RegistProtoCallBack(int msgType, int msgID, CMD_RUN_THREAD_TYPE runThreadType, ::google::protobuf::Message *pMsg, OnProtoMsgCallBack callBack) {
   /* if (msgType <= 0 || msgID <= 0) {
        XERR("MsgDispatcher::RegistProtoCallBack msgType <= 0 || msgID <= 0 [msgType:%d,msgID:%d]", msgType, msgID);
        return;
    }

    if (runThreadType < 0 || runThreadType >= CMD_RUN_THREAD_MAX) {
        XERR("MsgDispatcher::RegistProtoCallBack runThreadType < 0 || runThreadType >= CMD_RUN_THREAD_MAX [msgType:%d,msgID:%d]", msgType, msgID);
        return;
    }

    auto &idMap = m_mapProtoMsgCallBack[runThreadType][msgType * CMD_ID_PRIFX_ + msgID];
    idMap.first = pMsg;
    idMap.second = callBack;*/
}



void MsgDispatcher::RegistBinCallBack(int msgType, int msgID, CMD_RUN_THREAD_TYPE runThreadType, OnBinMsgCallBack callBack) {
   /* if (msgType <= 0 || msgID <= 0) {
        XERR("MsgDispatcher::RegistBinCallBack msgType <= 0 || msgID <= 0 [msgType:%d,msgID:%d]", msgType, msgID);
        return;
    }

    if (runThreadType < 0 || runThreadType >= CMD_RUN_THREAD_MAX) {
        XERR("MsgDispatcher::RegistBinCallBack runThreadType < 0 || runThreadType >= CMD_RUN_THREAD_MAX [msgType:%d,msgID:%d]", msgType, msgID);
        return;
    }

    m_mapBinMsgCallBack[runThreadType][msgType * CMD_ID_PRIFX_ + msgID] = callBack;*/
}


bool MsgDispatcher::OnSystemInternalMsg(uint8_t cmdId, uint8_t param, char* data, uint32_t len ) 
{
	//XLOG("MsgDispatcher::OnSystemInternalMsg[cmdId:%d,param:%d,len:%d]", cmdId, param, len);
    /*
    switch (param)
    {
    case PB_LOBBYGATE_SEND_MSG_2_PUBLIC_ENTITY:
    {//ϵͳ�ڲ�ʵ�����Ϣ
		//XLOG("MsgDispatcher::OnSystemInternalMsg PB_LOBBYGATE_SEND_MSG_2_PUBLIC_ENTITY![cmdId:%d,param:%d,len:%d]", cmdId, param, len);
        PBLobbyGateForwardMsg2Entity msg;
        if (!msg.ParseFromArray(data, len)) {
			XERR("MsgDispatcher::OnSystemInternalMsg PB_LOBBYGATE_SEND_MSG_2_PUBLIC_ENTITY Error![cmdId:%d,param:%d,len:%d]", cmdId, param, len);
            return false;
        }
        return EntityMgr::getMe().OnDispatchMsg(&msg);
    }break;

    case PB_LOBBYGATE_ADD_PUBLIC_ENTITY:
    {//����ȫ��ʵ���֪ͨ
        PBLobbyGateAddGlobalEntity msg;
        if (!msg.ParseFromArray(data, len)) {
			XERR("MsgDispatcher::OnSystemInternalMsg PB_LOBBYGATE_ADD_PUBLIC_ENTITY Error![cmdId:%d,param:%d,len:%d]", cmdId, param, len);
            return false;
        }
        EntityMgr::getMe().UpdateGlobalEntity(&msg);
        return true;
    }break;

    case PB_LOBBYGATE_REGISTER_PUBLIC_ENTITY_REQ:
    {//ע��ʵ������
        PBLobbyGateRegisterGlobalEntityReq msg;
        if (!msg.ParseFromArray(data, len)) {
			XERR("MsgDispatcher::OnSystemInternalMsg PB_LOBBYGATE_REGISTER_PUBLIC_ENTITY_REQ Error![cmdId:%d,param:%d,len:%d]", cmdId, param, len);
            return false;
        }

        EntityRegisterMgr::getMe().RegisterGlobalEntityReq(&msg);
        return true;
    }break;
    case PB_LOBBYGATE_REGISTER_PUBLIC_ENTITY_RSP:
    {//ע��ʵ��Ľ������
        PBLobbyGateRegisterGlobalEntityRsp msg;
        if (!msg.ParseFromArray(data, len)) {
			XERR("MsgDispatcher::OnSystemInternalMsg PB_LOBBYGATE_REGISTER_PUBLIC_ENTITY_RSP Error![cmdId:%d,param:%d,len:%d]", cmdId, param, len);
            return false;
        }
        EntityRegisterMgr::getMe().RegisterGlobalEntityRsp(&msg);
        return true;
    }break;

    case PB_LOBBYGATE_CREATE_ENTITY_ANYWHERE_REQ:
    {//����LobbyCenterͨ�����ؾ�����ĳ��lobbygate�ϴ���һ��ʵ��
        PBLobbyGateCreateEntityAnyWhere msg;
        if (!msg.ParseFromArray(data, len)) {
			XERR("MsgDispatcher::OnSystemInternalMsg PB_LOBBYGATE_CREATE_ENTITY_ANYWHERE_REQ Error![cmdId:%d,param:%d,len:%d]", cmdId, param, len);
            return false;
        }
        return EntityRegisterMgr::getMe().CreateEntityAnyWhereReq(&msg);
    }break;

    case PB_LOBBYGATE_CREATE_ENTITY_FROM_LOBBYCENTER:
    {//lobbycenter֪ͨĳ��lobbygate����ָ��һ��ʵ��
        PBLobbyGateCreateEntityFromCenter msg;
        if (!msg.ParseFromArray(data, len)) {
			XERR("MsgDispatcher::OnSystemInternalMsg PB_LOBBYGATE_CREATE_ENTITY_FROM_LOBBYCENTER Error![cmdId:%d,param:%d,len:%d]", cmdId, param, len);
            return false;
        }
        return EntityRegisterMgr::getMe().CreateEntityFromCenterReq(&msg);
    }break;

    case PB_All_Server_Connected:
    {//lobbycenter֪ͨĳ��lobbygate����ָ��һ��ʵ��
        PBAllServerConnected msg;
        if (!msg.ParseFromArray(data, len)) {
			XERR("MsgDispatcher::OnSystemInternalMsg PB_All_Server_Connected Error![cmdId:%d,param:%d,len:%d]", cmdId, param, len);
            return false;
        }
        return EntityRegisterMgr::getMe().AllServerConnected(&msg);
    }break;

    case PB_All_Server_StartNow:
    {//lobbycenter֪ͨĳ��lobbygate����ָ��һ��ʵ��

		static bool s_init = false;
		if (s_init)
		{
			XERR("OnAllServerConnected Failed! had Init!");
			return false;
		}
		s_init = true;

        PBAllServerStartNow msg;
        if (!msg.ParseFromArray(data, len)) {
			XERR("MsgDispatcher::OnSystemInternalMsg PB_All_Server_StartNow Error![cmdId:%d,param:%d,len:%d]", cmdId, param, len);
            return false;
        }
        XLOG("OnAllServerConnected!");
        GetServer()->OnAllServerConnected();
        return true;
    }break;
    default:
        break;
    }*/
    return false;
}

bool MsgDispatcher::OnMsg(UInt32 sguid, CMD_RUN_THREAD_TYPE runThreadType, xNetProcessor *pNetProcess, const unsigned char *buffer, unsigned int len) {
   /*
    xCommandSendBuffer *pCmd = (xCommandSendBuffer*)buffer;

    if (runThreadType < 0 || runThreadType >= CMD_RUN_THREAD_MAX) {
        XERR("MsgDispatcher::OnMsg runThreadType < 0 || runThreadType >= CMD_RUN_THREAD_MAX [sguid:%d,runThreadType:%d]", sguid, runThreadType);
        return false;
    }

	//XLOG("MsgDispatcher::OnMsg[runThreadType:%d,cmdId:%d,param:%d,len:%d]", runThreadType, pCmd->cmdID.cmd, pCmd->cmdID.param, pCmd->len);

    //ϵͳ�ڲ���Ϣ���ȴ���
    if (PB_SYSTEM_INTERNAL_CMD == pCmd->cmdID.cmd)
    {
        if (CMD_RUN_THREAD_NET == runThreadType)
            return false;
        //(ֻ�������߼��߳��ϴ���
        return OnSystemInternalMsg(pCmd->cmdID.cmd, pCmd->cmdID.param, pCmd->data, pCmd->len);
    }

    // ��Э���
    {
        auto idMap1 = m_mapBinMsgCallBack[runThreadType].find(pCmd->cmdID.cmd * CMD_ID_PRIFX_ + ALL_CMD_ID);
        if (idMap1 != m_mapBinMsgCallBack[runThreadType].end()) {

            (idMap1->second)(sguid, pNetProcess, buffer, len);
            return true;
        }
    }

    // СЭ���
    {
        auto idMap1 = m_mapBinMsgCallBack[runThreadType].find(pCmd->cmdID.cmd * CMD_ID_PRIFX_ + pCmd->cmdID.param);
        if (idMap1 != m_mapBinMsgCallBack[runThreadType].end()) {

            (idMap1->second)(sguid, pNetProcess, buffer, len);
            return true;
        }

        auto idMap = m_mapProtoMsgCallBack[runThreadType].find(pCmd->cmdID.cmd * CMD_ID_PRIFX_ + pCmd->cmdID.param);
        if (idMap == m_mapProtoMsgCallBack[runThreadType].end()) {
            // �����Ҳ������� ��Ϊ�������߳��ڴ���
            return false;
        }

        ::google::protobuf::Message *pMsg = idMap->second.first;
        if (!pMsg || !pMsg->ParseFromArray(pCmd->data, pCmd->len)) {
            return false;
        }

        if (pNetProcess)
        {
            pNetProcess->m_u32LastRecvTimestamp = time(0);
        }
        (idMap->second.second)(sguid, pNetProcess, pMsg);
        return true;
    }*/

    return false;
}
