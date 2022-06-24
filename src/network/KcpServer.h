#pragma once
#include <iostream>
#include <asio2/udp/udp_server.hpp>
#include "IBufferHandler.h"
struct KMsg
{
	IDataBuffer* pdata;
	std::shared_ptr<asio2::udp_session> s_ptr;
};

class KcpServer
{
public:
	KcpServer();
	~KcpServer();
	virtual void onConnect(std::shared_ptr<asio2::udp_session>& session_ptr);
	virtual void onClose(std::shared_ptr<asio2::udp_session>& session_ptr);
	virtual void onMsg(std::shared_ptr<asio2::udp_session>& session_ptr, IDataBuffer* pdata);
public:
	bool start(std::string host,int16_t port);
	void stop();
	bool runOneStep();

	std::function<void(uint32_t ConnID)> on_connect = nullptr;
	std::function<void(uint32_t ConnID)> on_close = nullptr;
	std::function<void(uint32_t ConnID,char* data,int32_t len)> on_msg = nullptr;
protected:
	asio2::udp_server server;

	std::list< std::shared_ptr<asio2::udp_session> >   m_lNewSession;            //待处理新连接列表
	std::list< std::shared_ptr<asio2::udp_session> >   m_lDelSession;			 //待处理断开连接列表
	std::mutex                  m_ConnListMutex;

	std::list<KMsg>                                        m_lMsg;
	std::mutex                                             m_MsgListMutex;

};

