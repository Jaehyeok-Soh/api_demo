#include "pch.h"
#include "CLoading.h"
#include "CTcpManager.h"
#include "CSceneManager.h"
#include "CPlayer.h"
#include "CScrollManager.h"
#include "CBmpManager.h"

CLoading::CLoading()
{
}

CLoading::~CLoading()
{
}

void CLoading::Update()
{
	if (m_arrObj[OBJ_PLAYER].size() >= 2)
	{
		CSceneManager::GetInstance()->SetChangeScene(true, SC_PLAY);
	}

	string msg = CTcpManager::GetInstance()->ListenSocket();
	if (msg != "" )//&& m_arrObj[OBJ_PLAYER].size() < 2)
	{
		json j = nlohmann::json::parse(msg);
		vector<DTOConnectInfo> connectInfos = j.get<vector<DTOConnectInfo>>();

		if (connectInfos.size() == 1 && m_arrObj[OBJ_PLAYER].size() == 0)
		{
			AddPlayer(connectInfos.front(), true);
		}
		
		if (connectInfos.size() > 1 && m_arrObj[OBJ_PLAYER].size() == 0)
		{
			AddPlayer(connectInfos.back(), true);
		}

		if(connectInfos.size() > 1 && m_arrObj[OBJ_PLAYER].size() == 1)
		{
			for (auto a : connectInfos)
			{
				if (a.netId
					!= static_cast<CPlayer*>(CSceneManager::GetInstance()->GetPlayer())->GetNetId())
					AddPlayer(a, false);
			}
		}
	}

	CScene::Update();
}

void CLoading::Render(HDC _dc)
{
	int iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = CScrollManager::Get_Instance()->Get_ScrollY();

	Render_Map(_dc, iScrollX, iScrollY);

	CScene::Render(_dc);
}

void CLoading::Enter()
{
	MapDC = CBmpManager::Get_Instance()->Find_Image(L"MapBig");
	SetMaxScroll(L"MapBig");

	Initialize();
}

void CLoading::Exit()
{
	DeleteAll();
}

void CLoading::Initialize()
{
	CTcpManager::GetInstance()->OpenSocket();
}

void CLoading::Render_Map(HDC _dc, int iScrollX, int iScrollY)
{
	GdiTransparentBlt(_dc,
		iScrollX,
		iScrollY,
		(int)1920,
		(int)1080,
		MapDC,
		0,
		0,
		(int)1920,	// 복사할 비트맵 가로 세로 사이즈
		(int)1080,
		RGB(255, 0, 255));	// 제거할 픽셀 색상 값
}

void CLoading::AddPlayer(DTOConnectInfo _playerConnectInfo, bool _isMine)
{
	//플레이어 초기화
	CObject* pPlayer = new CPlayer();
	//team에 따라 스폰 지점 변경
	pPlayer->SetName(L"Player");
	static_cast<CPlayer*>(pPlayer)->SetIsMine(_isMine);
	static_cast<CPlayer*>(pPlayer)->SetIsHost(_playerConnectInfo.isHost);
	static_cast<CPlayer*>(pPlayer)->SetNetId(_playerConnectInfo.netId);
	static_cast<CPlayer*>(pPlayer)->SetTeam(_playerConnectInfo.team);
	pPlayer->Initialize();
	AddObject(pPlayer, OBJ_PLAYER);

	if (_isMine)
	{
		RegisterPlayer(pPlayer);
		CSceneManager::GetInstance()->SetPlayer(pPlayer);
		//CSceneManager::GetInstance()->SetWeapon(pPlayer);
	}
	else
	{
		CSceneManager::GetInstance()->AddOtherPlayer(pPlayer);
	}
		
}
