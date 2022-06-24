#pragma once
#include "xDefine.h"
#include "asio2/udp/udp_session.hpp"
#include <unordered_map>
// 存放在udp_session userdata中,用来标记连接
struct ConnectData
{
	ConnectData()
	{
		connect_id = 0;
		uid = 0;
	}
	ConnectData(uint32_t cid, int64_t id)
		:connect_id(cid), uid(id)
	{
	 
	}
	uint32_t connect_id;
	int64_t  uid;
};


class CConnection
{
public:
	CConnection(std::shared_ptr<asio2::udp_session>& session_ptr);
	virtual ~CConnection();
	bool Stop();
	bool CheckHeader(char* pPacket, int32_t nLen);
	std::shared_ptr<asio2::udp_session> getSession() { return m_session; }
	uint32_t GetConnectID();
private:
	std::shared_ptr<asio2::udp_session> m_session;
	int32_t m_nCheckNo;
};

class CConnectMgr
{
private:
	CConnectMgr();
	virtual ~CConnectMgr();
public:
	static CConnectMgr* GetInstancePtr();

	CConnection* CreateConnection(std::shared_ptr<asio2::udp_session>& session_ptr);

	bool         DeleteConnection(uint32_t nConnID);

	CConnection* GetConnectionByID(uint32_t nConnID);

	bool         CloseAllConnection();

	bool         DestroyAllConnection();

	 

private:
	std::unordered_map<uint32_t,CConnection*> m_conns;
	uint32_t m_curid ;
};