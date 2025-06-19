#include "pch.h"
#include "CTcpManager.h"

CTcpManager* CTcpManager::m_pInstance = nullptr;
SOCKET CTcpManager::m_sock = 0;

CTcpManager::CTcpManager()
{
}

CTcpManager::~CTcpManager()
{
	CloseSocket();
}

void CTcpManager::Initialize()
{
	OpenSocket();
}

int CTcpManager::OpenSocket()
{
	WSADATA wsaData;
	m_sock = INVALID_SOCKET;
	sockaddr_in serverAddr;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(9000);

	if (InetPton(AF_INET, L"127.0.0.1", &serverAddr.sin_addr) != 1)
	{
		cerr << "IP conversion fail.\n";
		closesocket(m_sock);
		WSACleanup();
		return 1;
	}

	if (connect(m_sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		cerr << "Connect fail, Error code: " << WSAGetLastError() << "\n";
		CloseSocket();
		return 1;
	}

	u_long mode = 1;
	ioctlsocket(m_sock, FIONBIO, &mode);//논블로킹

	cout << "Server connected.\n";

	return 0;
}

void CTcpManager::SendSocket(string sendMsg)
{
	send(m_sock, sendMsg.c_str(), sendMsg.size(), 0);
}

string CTcpManager::ListenSocket()
{
	char buffer[1024] = {};
	int len = recv(m_sock, buffer, sizeof(buffer), 0);
	if (len == SOCKET_ERROR || WSAGetLastError() == WSAEWOULDBLOCK)
		return "";

	auto recMsg = string(buffer, len);
	cout << recMsg << "\n";

	return recMsg;
}

void CTcpManager::CloseSocket()
{
	closesocket(m_sock);
	WSACleanup();
}
