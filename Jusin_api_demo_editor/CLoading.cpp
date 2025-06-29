#include "pch.h"
#include "CLoading.h"
#include "CTcpManager.h"
#include "CSceneManager.h"
#include "CPlayer.h"
#include "CScrollManager.h"
#include "CBmpManager.h"
#include "CLoadingSpinner.h"
#include "CBlendingManager.h"
#include "CGameManager.h"

CLoading::CLoading()
{
}

CLoading::~CLoading()
{
	CBlendingManager::GetInstance()->Release();
}

void CLoading::Update()
{
	if (m_arrObj[OBJ_PLAYER].size() >= 2)
	{
		CSceneManager::GetInstance()->SetChangeScene(true, SC_PLAY);
	}

	try
	{
		string msg = CTcpManager::GetInstance()->ListenSocket();
		if (msg != "")//&& m_arrObj[OBJ_PLAYER].size() < 2)
		{
			json j = nlohmann::json::parse(msg);
			vector<DTOConnectInfo> connectInfos = j.get<vector<DTOConnectInfo>>();

			if (connectInfos.size() == 1 && m_arrObj[OBJ_PLAYER].size() == 0)
			{
				AddPlayer(connectInfos.front(), true);
			}

			if (connectInfos.size() > 1 && m_arrObj[OBJ_PLAYER].size() == 0)
			{
				AddPlayer(connectInfos.front(), false);
				AddPlayer(connectInfos.back(), true);
			}

			if (connectInfos.size() > 1 && m_arrObj[OBJ_PLAYER].size() == 1)
			{
				for (auto a : connectInfos)
				{
					if (a.netId
						!= static_cast<CPlayer*>(CSceneManager::GetInstance()->GetPlayer())->GetNetId())
						AddPlayer(a, false);
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "\n";
	}

	CScene::Update();
}

void CLoading::Render(HDC _dc)
{
	Render_Map(_dc);
	Render_LoadScreen(_dc);

	CScene::Render(_dc);
}

void CLoading::Enter()
{
	MapDC = CBmpManager::Get_Instance()->Find_Image(L"loadingBg");
	m_loadFrameBlueDC = CBmpManager::Get_Instance()->Find_Image(L"loadingFrameBlue");
	m_loadFrameRedDC = CBmpManager::Get_Instance()->Find_Image(L"loadingFrameRed");

	Initialize();
}

void CLoading::Exit()
{
	DeleteAll();
}

void CLoading::Initialize()
{
	CBlendingManager::GetInstance()->Initialize();

	CObject* pSpinner = new CLoadingSpinner();
	pSpinner->Initialize();
	pSpinner->SetPos(Vec2(635, 360));

	AddObject(pSpinner, OBJ_UI);

	CTcpManager::GetInstance()->OpenSocket();
}

void CLoading::Render_Map(HDC _dc)
{
	GdiTransparentBlt(_dc,
		0,
		0,
		WINCX,
		WINCY,
		MapDC,
		0,
		0,
		1024,	// 복사할 비트맵 가로 세로 사이즈
		512,
		RGB(1, 1, 1));	// 제거할 픽셀 색상 값
}

void CLoading::Render_LoadScreen(HDC _dc)
{
	const TCHAR* path_frameblue = L"../Image/ApiDemo/Client/loading/loadingFrameBlue.png";
	const TCHAR* path_loadScreen = L"../Image/ApiDemo/Client/loading/loadScreen.png";
	const TCHAR* path_FrameRed = L"../Image/ApiDemo/Client/loading/loadingFrameRed.png";

	if (m_arrObj[OBJ_PLAYER].size() == 0)
		return;
	else if (m_arrObj[OBJ_PLAYER].size() >= 1)
	{
		CBlendingManager::GetInstance()->Render(_dc, L"../Image/ApiDemo/Client/loading/loadingFrameBlue.png", Rect(557, 40, 153, 280), 0, 0, 308, 559, 1.f);
		CBlendingManager::GetInstance()->Render(_dc, L"../Image/ApiDemo/Client/loading/loadScreen.png", Rect(557, 40, 154, 281), 0, 0, 308, 561, 1.f);
		HDC blue = CBmpManager::Get_Instance()->Find_Image(L"swordman_attack_r");
		GdiTransparentBlt(_dc,
			557 + 8,
			40,
			153,
			281,
			blue,
			64 * 3,
			0,
			64,
			64,
			RGB(255, 255, 255));

		if (m_arrObj[OBJ_PLAYER].size() >= 2)
		{
			CBlendingManager::GetInstance()->Render(_dc, L"../Image/ApiDemo/Client/loading/loadingFrameRed.png", Rect(557, 400, 153, 280), 0, 0, 308, 559, 1.f);
			CBlendingManager::GetInstance()->Render(_dc, L"../Image/ApiDemo/Client/loading/loadScreen.png", Rect(557, 400, 154, 281), 0, 0, 308, 561, 1.f);
			HDC red = CBmpManager::Get_Instance()->Find_Image(L"acher_attack_l");
			GdiTransparentBlt(_dc,
				557 + 8,
				400,
				153,
				281,
				red,
				64 * 3,
				0,
				64,
				64,
				RGB(0, 0, 0));
		}
	}
}

void CLoading::AddPlayer(DTOConnectInfo _playerConnectInfo, bool _isMine)
{
	//플레이어 초기화
	CObject* pObj = new CPlayer();
	CPlayer* pPlayer = static_cast<CPlayer*>(pObj);
	pPlayer->SetName(L"Player");
	pPlayer->SetIsMine(_isMine);
	pPlayer->SetIsHost(_playerConnectInfo.isHost);
	pPlayer->SetNetId(_playerConnectInfo.netId);
	pPlayer->SetTeam(_playerConnectInfo.team);
	pPlayer->SetJob(CPlayer::JOB(_playerConnectInfo.job));

	if (_playerConnectInfo.team)
	{
		pPlayer->SetSpawnPos(Vec2(50.f, 300.f));

		if (pPlayer->GetIsMine())
		pPlayer->SetSpawnScroll(Vec2(-10.f, -1750.f));
	}
	else
	{
		pPlayer->SetSpawnPos(Vec2(652.f, 44.f));

		if (pPlayer->GetIsMine())
			pPlayer->SetSpawnScroll(Vec2(-469.f, 0.f));
	}

	//Initialize
	pPlayer->Initialize();
	AddObject(pPlayer, OBJ_PLAYER);

	if (_isMine)
	{
		pPlayer->SetAccount(CGameManager::GetInstance()->GetAccount().c_str());
		RegisterPlayer(pPlayer);
		CSceneManager::GetInstance()->SetPlayer(pPlayer);
	}
	else
	{
		CSceneManager::GetInstance()->AddOtherPlayer(pPlayer);
	}
}
