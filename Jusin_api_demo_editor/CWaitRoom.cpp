#include "pch.h"
#include "CWaitRoom.h"
#include "CBmpManager.h"
#include "CStartButton.h"

CWaitRoom::CWaitRoom()
{
}

CWaitRoom::~CWaitRoom()
{
}

void CWaitRoom::Initialize()
{
	MapDC = CBmpManager::Get_Instance()->Find_Image(L"waitRoomBg");

	CObject* pButtonStart = new CStartButton();
	pButtonStart->Initialize();
	pButtonStart->SetPos(Vec2(545.f, 696.f));
	pButtonStart->SetScale(Vec2(172.f, 44.f));
	pButtonStart->SetName(L"UI");
	pButtonStart->Set_FrameKey(L"startButton3");
	AddObject(pButtonStart, OBJ_UI);

	//m_LoginButton = static_cast<CStartButton*>(pButtonLogin);
}

void CWaitRoom::Update()
{
	CScene::Update();
}

void CWaitRoom::Render(HDC _dc)
{
	Render_Map(_dc);

	CScene::Render(_dc);
}

void CWaitRoom::Enter()
{
	Initialize();
}

void CWaitRoom::Exit()
{
	DeleteAll();
}

void CWaitRoom::Render_Map(HDC hdc)
{
	GdiTransparentBlt(hdc,
		0,
		0,
		WINCX,
		WINCY,
		MapDC,
		0,
		0,
		1280,	// 복사할 비트맵 가로 세로 사이즈
		720,//nice
		RGB(1, 1, 1));	// 제거할 픽셀 색상 값
}