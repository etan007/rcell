#include "KcpServer.h"
#include "Connection.h"
#include "DataBuffer.h"

KcpServer::KcpServer()
{
	server.bind_recv([&](std::shared_ptr<asio2::udp_session>& session_ptr, std::string_view data)
		{
			printf("recv : %zu %.*s\n", data.size(), (int)data.size(), data.data());
			// udp不考虑粘包的情况
			if (data.size() >= HEADER_LEN && data.size() < MAX_PACK_SIZE)
			{
				IDataBuffer* pDataBuffer = CBufferAllocator::GetInstancePtr()->AllocDataBuff(data.size());
				memcpy(pDataBuffer->GetBuffer(), data.data(), data.size());
				pDataBuffer->SetTotalLenth(data.size());
				KMsg kmsg = { pDataBuffer,session_ptr };
				m_MsgListMutex.lock();
				m_lMsg.push_back(kmsg);
				m_MsgListMutex.unlock();

			}
			else
			{
				XERR("bind_recv len err,data.size() = %d", (int)data.size());
			}
		
			
		});
	server.bind_connect([&](auto& session_ptr)
		{
			printf("client enter : %s %u %s %u\n",
				session_ptr->remote_address().c_str(), session_ptr->remote_port(),
				session_ptr->local_address().c_str(), session_ptr->local_port());
			m_ConnListMutex.lock();
			m_lNewSession.push_back(session_ptr);
			m_ConnListMutex.unlock();
		});
	server.bind_disconnect([&](auto& session_ptr)
		{
			printf("client leave : %s %u %s\n",
				session_ptr->remote_address().c_str(), session_ptr->remote_port(),
				asio2::last_error_msg().c_str());
			m_ConnListMutex.lock();
			m_lDelSession.push_back(session_ptr);
			m_ConnListMutex.unlock();
		});
	server.bind_handshake([](auto& session_ptr)
		{
			printf("client handshake : %s %u %d %s\n",
				session_ptr->remote_address().c_str(), session_ptr->remote_port(),
				asio2::last_error_val(), asio2::last_error_msg().c_str());
		});
	server.bind_start([&]()
		{
			if (asio2::get_last_error())
				printf("start udp server kcp failure : %d %s\n",
					asio2::last_error_val(), asio2::last_error_msg().c_str());
			else
				printf("start udp server kcp success : %s %u\n",
					server.listen_address().c_str(), server.listen_port());
		});
	server.bind_stop([&]()
		{
			printf("stop udp server kcp : %d %s\n",
				asio2::last_error_val(), asio2::last_error_msg().c_str());
		});
	server.bind_init([&]()
		{
			//// Join the multicast group. you can set this option in the on_init(_fire_init) function.
			//server.acceptor().set_option(
			//	// for ipv6, the host must be a ipv6 address like 0::0
			//	asio::ip::multicast::join_group(asio::ip::make_address("ff31::8000:1234")));
			//	// for ipv4, the host must be a ipv4 address like 0.0.0.0
			//	//asio::ip::multicast::join_group(asio::ip::make_address("239.255.0.1")));
		});
}
KcpServer::~KcpServer()
{

}

bool KcpServer::start(std::string host, int16_t port)
{
	return server.start(host, port, asio2::use_kcp);
	 
}

void KcpServer::stop()
{
	server.stop();
}

bool KcpServer::runOneStep()
{
	// 1.处理新连接
	m_ConnListMutex.lock();
	auto templNewSession = std::move(m_lNewSession);
	auto templDelSession = std::move(m_lDelSession);
	m_ConnListMutex.unlock();
	for (auto& iter : templNewSession)
	{
		onConnect(iter);
	}

	// 2.处理数据
	m_MsgListMutex.lock();
	auto templMsg = std::move(m_lMsg);
	m_MsgListMutex.unlock();
	for (auto& iter : templMsg)
	{
		std::string_view data();
		onMsg(iter.s_ptr,iter.pdata);
		iter.pdata->Release();
	}

	// 3.处理删除连接
	for (auto& iter : templDelSession)
	{
		onClose(iter);
	}

	return false;
}

void KcpServer::onConnect(std::shared_ptr<asio2::udp_session>& session_ptr) {
	CConnection* pConnection = CConnectMgr::GetInstancePtr()->CreateConnection(session_ptr);
	if (pConnection == nullptr)
	{
		XERR("CreateConnection==NULL");
		return;
	}
	if (on_connect)on_connect(pConnection->GetConnectID());
};

void KcpServer::onClose(std::shared_ptr<asio2::udp_session>& session_ptr) {

	ConnectData& data = session_ptr->get_user_data<ConnectData>();
	CConnectMgr::GetInstancePtr()->DeleteConnection(data.connect_id);
	if (on_close)on_close(data.connect_id);
};

void KcpServer::onMsg(std::shared_ptr<asio2::udp_session>& session_ptr, IDataBuffer* pdata) {
	ConnectData& data = session_ptr->get_user_data<ConnectData>();

	if (on_msg)on_msg(data.connect_id, pdata->GetBuffer(),pdata->GetTotalLenth());
};
