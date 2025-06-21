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
	WSADATA wsaData;
	m_sock = INVALID_SOCKET;
	sockaddr_in serverAddr;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(9000);
	
	if (InetPton(AF_INET, L"192.168.0.20", &serverAddr.sin_addr) != 1)//if (InetPton(AF_INET, L"127.0.0.1", &serverAddr.sin_addr) != 1)
	{
		cerr << "IP conversion fail.\n";
		closesocket(m_sock);
		WSACleanup();
		return -1;
	}

	if (connect(m_sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		cerr << "Connect fail, Error code: " << WSAGetLastError() << "\n";
		CloseSocket();
		return -1;
	}

	u_long mode = 1;
	ioctlsocket(m_sock, FIONBIO, &mode);//논블로킹

	cout << "Server connected.\n";

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

	recMsg = string(buffer, len);

	if (recMsg == "")
		return;

	cout << recMsg << "\n";

	try
	{
		nlohmann::json j = nlohmann::json::parse(recMsg);
		DTOPLAYER dto = j.get<DTOPLAYER>();

		for (auto a : CSceneManager::GetInstance()->GetOtherPlayer())
		{
			if (!static_cast<CPlayer*>(a)->GetIsMine() && static_cast<CPlayer*>(a)->GetNetId() == dto.m_iNetId)
			{
				a->SetPosX(dto.fX);
				a->SetPosY(dto.fY);
				static_cast<CPlayer*>(a)->SetState((CCharacter::STATE)dto.m_iState);
				static_cast<CPlayer*>(a)->SetFrameStart(dto.m_iFrameStart);

				wstring wstr(dto.m_strFrameKey.begin(), dto.m_strFrameKey.end());
				static_cast<CPlayer*>(a)->Set_FrameKey(wstr.c_str());

				static_cast<CPlayer*>(a)->SetDirection(dto.m_iDir);
			}
		}
	}
	catch (const std::exception&)
	{

	}
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

	cout << recMsg << "\n";

	return recMsg;
}

void CTcpManager::CloseSocket()
{
	closesocket(m_sock);
	WSACleanup();
}