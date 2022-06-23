#include "Connection.h"

CConnection::CConnection(std::shared_ptr<asio2::udp_session>& session_ptr)
	:m_session(session_ptr)
{
}

CConnection::~CConnection()
{
}

CConnectMgr::CConnectMgr()
{
}

CConnectMgr::~CConnectMgr()
{
}

CConnectMgr* CConnectMgr::GetInstancePtr()
{
	static CConnectMgr ConnectionMgr;

	return &ConnectionMgr;
}

CConnection* CConnectMgr::CreateConnection(std::shared_ptr<asio2::udp_session>& session_ptr)
{
	return nullptr;
}

bool CConnectMgr::DeleteConnection(int32_t nConnID)
{
	return false;
}

CConnection* CConnectMgr::GetConnectionByID(int32_t nConnID)
{
	return nullptr;
}

bool CConnectMgr::CloseAllConnection()
{
	return false;
}

bool CConnectMgr::DestroyAllConnection()
{
	return false;
}
