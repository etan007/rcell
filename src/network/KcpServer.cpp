#include "KcpServer.h"

KcpServer::KcpServer()
{
	server.bind_recv([](std::shared_ptr<asio2::udp_session>& session_ptr, std::string_view data)
		{
			//session_ptr->set_user_data(111);
			printf("recv : %zu %.*s\n", data.size(), (int)data.size(), data.data());

			
		});
	server.bind_connect([&](auto& session_ptr)
		{
			printf("client enter : %s %u %s %u\n",
				session_ptr->remote_address().c_str(), session_ptr->remote_port(),
				session_ptr->local_address().c_str(), session_ptr->local_port());
			m_ConnListMutex.lock();
			m_vtNewSession.push_back(session_ptr);
			m_ConnListMutex.unlock();
		});
	server.bind_disconnect([](auto& session_ptr)
		{
			printf("client leave : %s %u %s\n",
				session_ptr->remote_address().c_str(), session_ptr->remote_port(),
				asio2::last_error_msg().c_str());
			m_ConnListMutex.lock();
			m_vtDelSession.push_back(session_ptr);
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
	// 处理新连接
	// 处理数据
	// 处理删除连接
	return false;
}
