
#include "TcpClient.h"




TcpClient::TcpClient()
{
	 
}

TcpClient::~TcpClient()
{
	if (m_tcpClient)
	{
		delete m_tcpClient;
		m_tcpClient = nullptr;
	}
}

void TcpClient::Init(int reconnect)
{
	if (m_tcpClient)
	{
		delete m_tcpClient;
		m_tcpClient = nullptr;
	}
	m_tcpClient = new asio2::tcp_client();

	if (reconnect > 0)
	{
		m_tcpClient->auto_reconnect(true, std::chrono::milliseconds(1000)*reconnect); // enable auto reconnect and use custom delay
	}
	else
	{
		m_tcpClient->auto_reconnect(false);
	}

	// 因为所有客户端只有一个线程，所以除了init事件是在main线程中触发的，所有
	// 其它的事件，包括recv,timer,post事件等，都是在“线程0”中触发的。
	m_tcpClient->bind_init([&]()
		{
			// 固定在“线程main”中触发,也即调用线程

		});
	m_tcpClient->bind_connect([&](asio::error_code ec)
		{
			int errcode = asio2::last_error_val();
			std::string  errmsg = asio2::last_error_msg().c_str();
			auto msgob = new TCPMsgOb();
			msgob->evType = eOnConnect;
			if (asio2::get_last_error())
			{
				
				msgob->strbuf = errmsg;
				printf("connect failure : %d %s\n", asio2::last_error_val(), asio2::last_error_msg().c_str());
			}
			else
			{
				msgob->nerr = 0;
				msgob->strbuf = "sucess";
				printf("connect success : %s %u\n", m_tcpClient->local_address().c_str(), m_tcpClient->local_port());
			}
			{
				std::lock_guard<std::mutex> lock(_mutex_msg);
				_msgQueue.push(msgob);
			}
			 
		});

	m_tcpClient->bind_disconnect([&](asio::error_code ec)
			{
			    int errcode = asio2::last_error_val();
				std::string  errmsg = asio2::last_error_msg().c_str();
				auto msgob = new TCPMsgOb();
				msgob->evType = eOnDisconnect;
				msgob->strbuf = errmsg;
				msgob->nerr = errcode;
			    
				{
					std::lock_guard<std::mutex> lock(_mutex_msg);
					_msgQueue.push(msgob);
				}
				printf("disconnect : %d %s\n", asio2::last_error_val(), asio2::last_error_msg().c_str());
				
		});
	m_tcpClient->bind_recv([&](std::string_view sv)
		{
			auto msgob = new TCPMsgOb;
			msgob->evType = eOnRecv;
			msgob->strbuf = sv;
			{
				std::lock_guard<std::mutex> lock(_mutex_msg);
				_msgQueue.push(msgob);
			}

			printf("recv : %u %.*s\n", (unsigned)sv.size(), (int)sv.size(), sv.data());
			

		});


}

void TcpClient::Connect(std::string host, unsigned short port)
{
	if (m_tcpClient)
	   m_tcpClient->async_start(host, port);
}

bool TcpClient::Send(std::string_view view)
{
	if (m_tcpClient && m_tcpClient->is_started())
	m_tcpClient->send(view.data(), view.size());
	return false;
}

bool TcpClient::CloseSocket()
{
	if (m_tcpClient)
		m_tcpClient->stop();
	return false;
}

bool TcpClient::PeekMsg()
{
	if (!_msgQueue.empty())
	{
		TCPMsgOb* msg = nullptr;
		{
			std::lock_guard<std::mutex> lock(_mutex_msg);
			msg = _msgQueue.front();
		}
		switch (msg->evType)
		{
		case eOnConnect:
			if (OnConnectCB)
			{
				OnConnectCB(msg->nerr, msg->strbuf);
			}
			break;
		case eOnDisconnect:
			if (OnDisconnectCB)
			{
				OnDisconnectCB(msg->nerr, msg->strbuf);
			}
			break;
		case eOnRecv:
			if (OnRecvCB)
			{
				if (OnRecvCB)
				{
					std::string_view sv = msg->strbuf;
					OnRecvCB(sv);
				}
			}
			break;
		}
		_msgQueue.pop();
		delete msg;
		return true;
	}
	return false;
}




