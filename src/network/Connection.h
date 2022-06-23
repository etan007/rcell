#pragma once
#include "xDefine.h"
#include "asio2/udp/udp_session.hpp"
#include <unordered_map>
// 存放在udp_session userdata中,用来标记连接
struct ConnectData
{
	int32_t connect_id;
	int64_t  uid;
};


class CConnection
{
public:
	CConnection(std::shared_ptr<asio2::udp_session>& session_ptr);
	virtual ~CConnection();
private:
	std::shared_ptr<asio2::udp_session> m_session;
};

class CConnectMgr
{
private:
	CConnectMgr();
	virtual ~CConnectMgr();
public:
	static CConnectMgr* GetInstancePtr();

	CConnection* CreateConnection(std::shared_ptr<asio2::udp_session>& session_ptr);

	bool         DeleteConnection(int32_t nConnID);

	CConnection* GetConnectionByID(int32_t nConnID);

	bool         CloseAllConnection();

	bool         DestroyAllConnection();

	 

private:
	std::unordered_map<int32_t,CConnection*> m_conns;
};