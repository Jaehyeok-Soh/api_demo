#include "pch.h"
#include "CTcpManager.h"
#include "DTOPlayer.h"
#include "CPlayer.h"
#include "CSceneManager.h"
#include "DTOConnectInfo.h"

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
	//WSADATA wsaData;
	//m_sock = INVALID_SOCKET;
	//sockaddr_in serverAddr;

	//WSAStartup(MAKEWORD(2, 2), &wsaData);

	//m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//serverAddr.sin_family = AF_INET;
	//serverAddr.sin_port = htons(9000);
	//
	///*TODO: ref config file
	//sso550.ddns.net*/
	//if (InetPton(AF_INET, L"192.168.0.183", &serverAddr.sin_addr) != 1)//if (InetPton(AF_INET, L"127.0.0.1", &serverAddr.sin_addr) != 1)
	//{
	//	cerr << "IP conversion fail.\n";
	//	closesocket(m_sock);
	//	WSACleanup();
	//	return -1;
	//}

	//if (connect(m_sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	//{
	//	cerr << "Connect fail, Error code: " << WSAGetLastError() << "\n";
	//	CloseSocket();
	//	return -1;
	//}

	//u_long mode = 1;
	//ioctlsocket(m_sock, FIONBIO, &mode);//논블로킹

	//cout << "Server connected.\n";

	//return 1;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return -1;

	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET)
		return -1;

	sockaddr_in serverAddr{};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(9000);

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

	if (connect(m_sock,
		(sockaddr*)&serverAddr,
		sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cerr << "Connect fail, err "
			<< WSAGetLastError() << '\n';
		closesocket(m_sock);
		WSACleanup();
		return -1;
	}

	u_long mode = 1;                    // 논블로킹
	if (ioctlsocket(m_sock, FIONBIO, &mode) != NO_ERROR)
	{
		std::cerr << "Failed to set non-blocking mode.\n";
		closesocket(m_sock); WSACleanup(); return -1;
	}

	std::cout << "Server connected.\n";
	return 1;
}

DTOConnectInfo CTcpManager::JoinSeq()
{
	char buffer[4096] = {};

	int len = recv(m_sock, buffer, sizeof(buffer), 0);
	if (len == SOCKET_ERROR || WSAGetLastError() == WSAEWOULDBLOCK)
		return DTOConnectInfo();

	auto recMsg = string(buffer, len);
	cout << recMsg << "\n";
	json j = nlohmann::json::parse(recMsg);

	return j.get<DTOConnectInfo>();
}

void CTcpManager::SendSocket(string sendMsg)
{
	send(m_sock, sendMsg.c_str(), sendMsg.size(), 0);
}

void CTcpManager::SyncPlay()
{
	char buffer[4096] = {};
	string recMsg;
	int len = recv(m_sock, buffer, sizeof(buffer), 0);
	if (len == SOCKET_ERROR || WSAGetLastError() == WSAEWOULDBLOCK)
		return;

	if (len == 0) {
		std::cerr << "close\n";
		return;
	}

	recMsg = string(buffer, len);

	if (recMsg == "")
		return;

	stringstream ss(recMsg);
	string line;

	while (getline(ss, line)) {
		if (line.empty()) continue;

		try
		{
			nlohmann::json j = nlohmann::json::parse(line);
			DTOPLAYER dto = j.get<DTOPLAYER>();
			// 플레이어 위치, 상태 업데이트
			auto players = CSceneManager::GetInstance()->GetOtherPlayer();
			for (auto a : players)
			{
				CPlayer* player = static_cast<CPlayer*>(a);
				if (!static_cast<CPlayer*>(a)->GetIsMine() && static_cast<CPlayer*>(a)->GetNetId() == dto.m_iNetId)
				{
					player->SetPosX(dto.fX);
					player->SetPosY(dto.fY);
					player->SetState((CCharacter::STATE)dto.m_iState);
					player->SetFrameStart(dto.m_iFrameStart);

					wstring wstr(dto.m_strFrameKey.begin(), dto.m_strFrameKey.end());
					player->Set_FrameKey(wstr.c_str());
					player->SetDirection(dto.m_iDir);
				}
			}
			break;
		}
		catch (const std::exception& e)
		{
			std::cerr << "Parse fail " << e.what() << "\nJSON\n" << line << "\n";
		}
	}

	//try
	//{
	//	nlohmann::json j = nlohmann::json::parse(recMsg);
	//	DTOPLAYER dto = j.get<DTOPLAYER>();

	//	auto players = CSceneManager::GetInstance()->GetOtherPlayer();
	//	for (auto a : players)
	//	{
	//		CPlayer* player = static_cast<CPlayer*>(a);
	//		if (!static_cast<CPlayer*>(a)->GetIsMine() && static_cast<CPlayer*>(a)->GetNetId() == dto.m_iNetId)
	//		{
	//			player->SetPosX(dto.fX);
	//			player->SetPosY(dto.fY);
	//			player->SetState((CCharacter::STATE)dto.m_iState);
	//			player->SetFrameStart(dto.m_iFrameStart);

	//			wstring wstr(dto.m_strFrameKey.begin(), dto.m_strFrameKey.end());
	//			player->Set_FrameKey(wstr.c_str());
	//			player->SetDirection(dto.m_iDir);
	//		}
	//	}
	//}
	//catch (const std::exception& ex)
	//{
	//	cout << ex.what() << "\n";
	//}
}

string CTcpManager::ListenSocket()
{
	char buffer[4096] = {};
	int len = recv(m_sock, buffer, sizeof(buffer), 0);
	if (len == SOCKET_ERROR || WSAGetLastError() == WSAEWOULDBLOCK)
		return "";

	auto recMsg = string(buffer, len);

	if (recMsg == "")
		return recMsg;

	return recMsg;
}

void CTcpManager::CloseSocket()
{
	closesocket(m_sock);
	WSACleanup();
}