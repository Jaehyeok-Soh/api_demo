#include "pch.h"
#include "CLoading.h"
#include "CTcpManager.h"
#include "CSceneManager.h"
#include "CPlayer.h"
#include "CScrollManager.h"
#include "CBmpManager.h"
#include "CLoadingSpinner.h"

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
	//m_loadScreenBlueDC = CBmpManager::Get_Instance()->Find_Image(L"loadScreen");
	//m_loadScreenRedDC = CBmpManager::Get_Instance()->Find_Image(L"loadScreen");
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
	HDC m_loadScreenBlueDC = CBmpManager::Get_Instance()->Find_Image(L"loadScreen");
	HDC blue = CBmpManager::Get_Instance()->Find_Image(L"swordman_attack_r");

	GdiTransparentBlt(m_loadScreenBlueDC,
		0,
		0,
		309,
		559,
		blue,
		64 * 3,
		0,
		64,
		64,
		RGB(255, 255, 255));

	GdiTransparentBlt(m_loadScreenBlueDC,
		0,
		0,
		309,
		559,
		m_loadFrameBlueDC,
		0,
		0,
		308,
		560,
		RGB(255, 255, 255));

	GdiTransparentBlt(_dc,
		557,
		40,
		154,
		281,
		m_loadScreenBlueDC,
		0,
		0,
		308,
		561,
		RGB(255, 255, 255));

	HDC hDC = GetDC(g_hWnd);

	HDC m_loadScreenRedDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);
	const TCHAR* path = L"../Image/ApiDemo/Client/loading/loadScreen.bmp";
	HBITMAP hBitMap = (HBITMAP)LoadImage(NULL,
		path,
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	HBITMAP m_hOldMap = (HBITMAP)SelectObject(m_loadScreenRedDC, hBitMap);

	//m_loadScreenRedDC = CBmpManager::Get_Instance()->Find_Image(L"loadScreen");
	HDC red = CBmpManager::Get_Instance()->Find_Image(L"swordman_attack_l");
	GdiTransparentBlt(m_loadScreenRedDC,
		0,
		0,
		309,
		559,
		red,
		0,
		0,
		64,
		64,
		RGB(255, 255, 255));

	GdiTransparentBlt(m_loadScreenRedDC,
		0,
		0,
		309,
		559,
		m_loadFrameRedDC,
		0,
		0,
		308,
		560,
		RGB(255, 255, 255));

	GdiTransparentBlt(_dc,
		557,
		400,
		154,
		281,
		m_loadScreenRedDC,
		0,
		0,
		308,
		561,
		RGB(255, 255, 255));

	//SelectObject(hDC, m_hOldMap); // 이전 비트맵 복원
	DeleteObject(hBitMap);
	DeleteDC(hDC);
}

void CLoading::AddPlayer(DTOConnectInfo _playerConnectInfo, bool _isMine)
{
	//플레이어 초기화
	CObject* pObj = new CPlayer();
	CPlayer* pPlayer = static_cast<CPlayer*>(pObj);
	//team에 따라 스폰 지점 변경
	pPlayer->SetName(L"Player");
	pPlayer->SetIsMine(_isMine);
	pPlayer->SetIsHost(_playerConnectInfo.isHost);
	pPlayer->SetNetId(_playerConnectInfo.netId);
	pPlayer->SetTeam(_playerConnectInfo.team);
	pPlayer->SetJob(CPlayer::JOB(_playerConnectInfo.job));
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
