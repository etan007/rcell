#pragma once
#include <string>
#include <string_view>
#include <functional>
#include <mutex>
#include <queue> // std::queue
#include <asio2/asio2.hpp>
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

	
	class TcpClient
	{
	public:
		TcpClient();
		~TcpClient();
	public:
		//���ӻص�������ţ�������
		typedef std::function<void(int, std::string)> OnConnectHandle;
		//�Ͽ����ӻص�������ţ�������
		typedef std::function<void(int, std::string)> OnDisconnectHandle;
		//���ݽ��ջص�
		typedef std::function<void(std::string_view sv)> OnRecvHandle;
	public:
		//��ʼ�����磨����ʱ��(��)��
		void Init(int reconnect);
		//���ӷ���������ʼ����
		void Connect(std::string host, unsigned short port);
		//��������
		//bool Send(char* data, int len);
		bool Send(std::string_view view);
		//�ر�����
		bool CloseSocket();
		//�������ֻ����Ϸ�̵߳���
		bool PeekMsg();
		
	public:
		OnDisconnectHandle OnDisconnectCB = nullptr;
		OnConnectHandle OnConnectCB = nullptr;
		OnRecvHandle OnRecvCB = nullptr;
	private:
		asio2::tcp_client* m_tcpClient;
		std::mutex					            _mutex_msg;  // _mutex_msg ��Ϣ��
		std::queue<TCPMsgOb*>					_msgQueue;    //��Ϣ����
	};


