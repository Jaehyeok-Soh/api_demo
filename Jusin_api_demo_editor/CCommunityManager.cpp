#include "pch.h"
#include "CCommunityManager.h"
CCommunityManager* CCommunityManager::m_pInstance = nullptr;
SOCKET CCommunityManager::m_Csock = 0;

CCommunityManager::CCommunityManager()
{
}

CCommunityManager::~CCommunityManager()
{
	CloseSocket();
}

void CCommunityManager::Initialize()
{
	OpenSocket();
}

int CCommunityManager::OpenSocket()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return -1;

	m_Csock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Csock == INVALID_SOCKET)
		return -1;

	sockaddr_in serverAddr{};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(9005);

	//도메인 → IP
	addrinfoW hints{};
	hints.ai_family = AF_INET;       // IPv4
	hints.ai_socktype = SOCK_STREAM;

	addrinfoW* res = nullptr;

	//int ret = GetAddrInfoW(L"sso550.ddns.net", L"9000", &hints, &res);
	//int ret = GetAddrInfoW(L"192.168.219.182", L"9000", &hints, &res);
	int ret = GetAddrInfoW(L"192.168.219.152", L"9000", &hints, &res);

	if (ret != 0 || !res)
	{
		wcerr << L"Resolve failed: " << gai_strerrorW(ret) << '\n';
		std::cerr << "Domain resolve failed: " << ret << '\n';
		WSACleanup();
		return -1;
	}
	serverAddr.sin_addr =
		((sockaddr_in*)res->ai_addr)->sin_addr;
	FreeAddrInfoW(res);
	//

	if (connect(m_Csock,
		(sockaddr*)&serverAddr,
		sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cerr << "Connect fail, err "
			<< WSAGetLastError() << '\n';
		closesocket(m_Csock);
		WSACleanup();
		return -1;
	}

	u_long mode = 1;                    // 논블로킹
	if (ioctlsocket(m_Csock, FIONBIO, &mode) != NO_ERROR)
	{
		std::cerr << "Failed to set non-blocking mode.\n";
		closesocket(m_Csock); WSACleanup(); return -1;
	}

	std::cout << "Server connected.\n";
	return 1;
}

void CCommunityManager::SendSocket(string sendMsg)
{
}

string CCommunityManager::ListenSocket()
{
	return string();
}

void CCommunityManager::CloseSocket()
{
}
