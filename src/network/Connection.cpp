#include "Connection.h"
#include "PacketHeader.h"

CConnection::CConnection(std::shared_ptr<asio2::udp_session>& session_ptr)
	:m_session(session_ptr), m_nCheckNo(0)
{
}

CConnection::~CConnection()
{
	Stop();
}

bool CConnection::Stop()
{
	if (m_session)
	{
		m_session->stop();
		return true;
	}
	return false;
}

bool CConnection::CheckHeader(char* pPacket,int32_t nLen)
{
	/*
	1.首先验证包的验证吗
	2.包的长度
	3.包的序号
	*/
	if (nLen < HEADER_LEN)
	{
		return false;
	}
	PacketHeader* pHeader = (PacketHeader*)pPacket;
	if (pHeader->CheckCode != CODE_VALUE)
	{
		return false;
	}

	if (pHeader->nSize > MAX_PACK_SIZE)
	{
		return false;
	}
	/*
	if (pHeader->nSize <= 0)
	{
		CLog::GetInstancePtr()->LogError("验证-失败 pHeader->nSize <= 0, pHeader->nMsgID:%d", pHeader->nSize, pHeader->nMsgID);
		return FALSE;
	}

	if (pHeader->nMsgID > 399999 || pHeader->nMsgID == 0)
	{
		return FALSE;
	}*/

	if (m_nCheckNo == 0)
	{
		m_nCheckNo = pHeader->nPacketNo - (pHeader->nMsgID ^ pHeader->nSize) + 1;
		return true;
	}

	if (pHeader->nPacketNo == (pHeader->nMsgID ^ pHeader->nSize) + m_nCheckNo)
	{
		m_nCheckNo += 1;
		return false;
	}

	return false;
}

uint32_t CConnection::GetConnectID()
{
	if (m_session)
	{
		ConnectData& data = m_session->get_user_data<ConnectData>();
		return data.connect_id;
	}
	return 0;
}


CConnectMgr::CConnectMgr()
{
	m_curid = 0;
}

CConnectMgr::~CConnectMgr()
{
	DestroyAllConnection();
}

CConnectMgr* CConnectMgr::GetInstancePtr()
{
	static CConnectMgr ConnectionMgr;

	return &ConnectionMgr;
}

CConnection* CConnectMgr::CreateConnection(std::shared_ptr<asio2::udp_session>& session_ptr)
{
	CConnection* pConnection = new CConnection(session_ptr);
	if (pConnection)
	{
		//创建连接id
		uint32_t connect_id = ++m_curid;
		if (m_curid >= 0xffffffff)
		{
			m_curid = 1;
		}
		bool bfind = false;
		//从当前位置查找一个没有使用的id
		uint32_t nFindStart = m_curid;
		for (; m_curid < 0xffffffff; m_curid++)
		{
			if (m_conns.find(m_curid) != m_conns.end())
			{
				bfind = true;
				break;
			}
		}
		//从前往后查
		if (!bfind)
		{
			for (m_curid = 1; m_curid < nFindStart; m_curid++)
			{
				if (m_conns.find(m_curid) != m_conns.end())
				{
					bfind = true;
					break;
				}
			}
		}

		if (bfind)
		{
			m_conns[m_curid] = pConnection;
			ConnectData data = { m_curid,0 };
			session_ptr->set_user_data(data);
		}
		else
		{
			session_ptr->stop();
			delete pConnection;
		}
		
	}
	return nullptr;
}

bool CConnectMgr::DeleteConnection(uint32_t nConnID)
{
	auto iter = m_conns.find(m_curid);
	if (iter != m_conns.end())
	{
		delete iter->second;
		m_conns.erase(nConnID);
		return true;
	}
	return false;
}

CConnection* CConnectMgr::GetConnectionByID(uint32_t nConnID)
{
	auto iter = m_conns.find(m_curid);
	if (iter != m_conns.end())
	{
		return iter->second;
	}
	return nullptr;
}

bool CConnectMgr::CloseAllConnection()
{
	for (auto& iter : m_conns)
	{
		if (iter.second)
		{
			iter.second->Stop();
		}
	}
	return true;
}

bool CConnectMgr::DestroyAllConnection()
{
	for (auto& iter : m_conns)
	{
		if (iter.second)
		{
			delete iter.second;
		}
	}
	m_conns.clear();
	return false;
}
