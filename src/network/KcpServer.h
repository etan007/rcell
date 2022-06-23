#pragma once
#include <iostream>
#include <asio2/udp/udp_server.hpp>
class KcpServer
{
public:
	KcpServer();
	~KcpServer();
	virtual void onConnect(std::shared_ptr<asio2::udp_session>& session_ptr) {
		if(on_connect)on_connect(session_ptr);
	};
	virtual void onClose(std::shared_ptr<asio2::udp_session>& session_ptr) {
		if(on_close)on_close(session_ptr);
	};
	virtual void onMsg(std::shared_ptr<asio2::udp_session>& session_ptr, std::string_view data) {
		if(on_msg)on_msg(session_ptr, data);
	};
public:
	bool start(std::string host,int16_t port);
	void stop();
	bool runOneStep();

	std::function<void(std::shared_ptr<asio2::udp_session>&)> on_connect = nullptr;
	std::function<void(std::shared_ptr<asio2::udp_session>&)> on_close = nullptr;
	std::function<void(std::shared_ptr<asio2::udp_session>&, std::string_view data)> on_msg = nullptr;
protected:
	asio2::udp_server server;

	std::vector< std::shared_ptr<asio2::udp_session> >   m_vtNewSession;            //待处理新连接列表
	std::vector< std::shared_ptr<asio2::udp_session> >   m_vtDelSession;			//待处理断开连接列表
	std::mutex                  m_ConnListMutex;

};

