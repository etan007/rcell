#pragma once
#include <string>
#include <string_view>
#include <functional>
#include <mutex>
#include <queue> // std::queue
namespace asio2
{
	enum TCPMsgType
	{
	    eOnDisconnect = 1,
		eOnConnect,
		eOnRecv
	};
	struct TCPMsgOb
	{
		TCPMsgOb()
		{
			evType = 0;
			nerr = 0;
		}
		int evType;
		std::string strbuf;
		int nerr;
	};

	class tcp_client;
	class TcpClient
	{
		TcpClient();
		~TcpClient();
	public:
		//连接回调（错误号，描述）
		typedef std::function<void(int, std::string)> OnConnectHandle;
		//断开连接回调（错误号，描述）
		typedef std::function<void(int, std::string)> OnDisconnectHandle;
		//数据接收回调
		typedef std::function<void(std::string_view sv)> OnRecvHandle;
	public:
		//初始化网络（重连时间(秒)，连接回调，断开回调，数据接收回调）
		void Init(int reconnect, OnConnectHandle conCB, OnDisconnectHandle disCB, OnRecvHandle recvCB);
		//连接服务器，开始服务
		void Connect(std::string host, unsigned short port);
		//发送数据
		bool Send(char* data, int len);
		//关闭连接
		bool CloseSocket();
		//这个函数只能游戏线程调用
		bool PeekMsg();
		
	private:
		OnDisconnectHandle m_OnDisconnectCB = nullptr;
		OnConnectHandle m_OnConnectCB = nullptr;
		OnRecvHandle m_OnRecvCB = nullptr;
	private:
		asio2::tcp_client* m_tcpClient;
		std::mutex					            _mutex_msg;  // _mutex_msg 消息锁
		std::queue<TCPMsgOb*>					_msgQueue;    //消息队列
	};
}

