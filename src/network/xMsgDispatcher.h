#pragma once

//#include "xNetProcessor.h"
#include "xSingleton.h"
#include "xServerRouteDefine.h"

#include "google/protobuf/message.h"

class xNetProcessor
{};

typedef bool(*OnProtoMsgCallBack)(UInt32 sguid, xNetProcessor *pNetProcess, const ::google::protobuf::Message *);
typedef bool(*OnBinMsgCallBack)(UInt32 sguid, xNetProcessor *pNetProcess, const unsigned char *buff, unsigned int len);


// ��СЭ��ϲ�ǰ׺
#define CMD_ID_PRIFX_ 10000
// ��������СЭ��
#define ALL_CMD_ID (CMD_ID_PRIFX_-1)



// ����Э�鵽�ϲ��߼���ת��
class MsgDispatcher : public xSingleton<MsgDispatcher>
{
public:
    void RegistProtoCallBack(int msgType, int msgID, CMD_RUN_THREAD_TYPE runThreadType, ::google::protobuf::Message *, OnProtoMsgCallBack callBack);
    void RegistBinCallBack(int msgType, int msgID, CMD_RUN_THREAD_TYPE runThreadType, OnBinMsgCallBack callBack);

public:
    //����ϵͳ�ڲ���Ϣ
    bool OnSystemInternalMsg(uint8_t cmdId, uint8_t param, char * data, uint32_t len);
    bool OnMsg(UInt32 sguid, CMD_RUN_THREAD_TYPE runThreadType, xNetProcessor *pNetProcess, const unsigned char *buff, unsigned int len);

private:
    //map<int, std::pair<::google::protobuf::Message *, OnProtoMsgCallBack>> m_mapProtoMsgCallBack[CMD_RUN_THREAD_MAX];
    //map<int, OnBinMsgCallBack> m_mapBinMsgCallBack[CMD_RUN_THREAD_MAX];
};


// ע��Э�鴦��ص� ��ʼ
#define REGIST_MSG_CALLBACK_BEGIN(rname) \
struct RegistCmdCallBack_##rname{\
    RegistCmdCallBack_##rname() {\

// ע��Э�鴦��ص� ע��protoЭ�� �ӿ�
#define REGIST_PROTO_MSG_CALLBACK(threadType, pbmsg, callback){\
    pbmsg *_msg = new pbmsg();\
    MsgDispatcher::getMe().RegistProtoCallBack(_msg->cmdid().cmd(), _msg->param(), threadType, _msg, callback);}

// ע��Э�鴦��ص� ע�������Э�� �ӿ�
#define REGIST_BIN_MSG_CALLBACK(threadType, cmdid, msgid, callback)\
    MsgDispatcher::getMe().RegistBinCallBack(cmdid, msgid, threadType, callback);

// ע��Э�鴦��ص� ע�������Э�� ��Э��ID����Э��
#define REGIST_ALLID_BIN_MSG_CALLBACK(threadType, cmdid, callback)\
    MsgDispatcher::getMe().RegistBinCallBack(cmdid, ALL_CMD_ID, threadType, _msg, callback);

// ע��Э�鴦��ص� ����
#define REGIST_MSG_CALLBACK_END(rname) \
    }\
};\
RegistCmdCallBack_##rname _temp_reg_##rname;\


// ע������
//bool On_PB_CLIENT_CONECCT_CHECK_REQ_CMD(xNetProcessor *pNetProcess, const ::google::protobuf::Message &stMsg) {
//}
//BEGIN_REGIST_MSG_CALLBACK(PB_CLIENT)
//REGIST_MSG_CALLBACK(PBClientFriendDataUpdateCmd, On_PB_CLIENT_CONECCT_CHECK_REQ_CMD)
//END_REGIST_MSG_CALLBACK(PB_CLIENT)

