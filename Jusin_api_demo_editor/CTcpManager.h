#pragma once
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class CTcpManager
{
public:
	CTcpManager();
	~CTcpManager();

	CTcpManager(CTcpManager&) = delete;
	CTcpManager(CTcpManager&&) = delete;
	CTcpManager& operator()(CTcpManager&) = delete;
	CTcpManager& operator()(CTcpManager&&) = delete;

public:
	static CTcpManager* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTcpManager();
		}
		return m_pInstance;
	}

	static void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void Initialize();

	int OpenSocket();
	void SendSocket(string sendMsg);
	string ListenSocket();
	static void CloseSocket();	

private:
	static CTcpManager* m_pInstance;
	static SOCKET m_sock;
};

