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
		//���ӻص�������ţ�������
		typedef std::function<void(int, std::string)> OnConnectHandle;
		//�Ͽ����ӻص�������ţ�������
		typedef std::function<void(int, std::string)> OnDisconnectHandle;
		//���ݽ��ջص�
		typedef std::function<void(std::string_view sv)> OnRecvHandle;
	public:
		//��ʼ�����磨����ʱ��(��)�����ӻص����Ͽ��ص������ݽ��ջص���
		void Init(int reconnect, OnConnectHandle conCB, OnDisconnectHandle disCB, OnRecvHandle recvCB);
		//���ӷ���������ʼ����
		void Connect(std::string host, unsigned short port);
		//��������
		bool Send(char* data, int len);
		//�ر�����
		bool CloseSocket();
		//�������ֻ����Ϸ�̵߳���
		bool PeekMsg();
		
	private:
		OnDisconnectHandle m_OnDisconnectCB = nullptr;
		OnConnectHandle m_OnConnectCB = nullptr;
		OnRecvHandle m_OnRecvCB = nullptr;
	private:
		asio2::tcp_client* m_tcpClient;
		std::mutex					            _mutex_msg;  // _mutex_msg ��Ϣ��
		std::queue<TCPMsgOb*>					_msgQueue;    //��Ϣ����
	};
}

