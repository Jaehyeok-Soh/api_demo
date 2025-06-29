#include "pch.h"
#include "CLobby.h"
#include "CBmpManager.h"
#include "CStartButton.h"
#include "CGameManager.h"

CLobby::CLobby()
{
}

CLobby::~CLobby()
{
}

void CLobby::Initialize()
{
	MapDC = CBmpManager::Get_Instance()->Find_Image(L"lobbyBg");

	CObject* pButtonStart = new CStartButton();
	pButtonStart->Initialize();
	pButtonStart->SetPos(Vec2(142.f, 42.f));
	pButtonStart->SetName(L"UI");
	pButtonStart->Set_FrameKey(L"startButton");
	AddObject(pButtonStart, OBJ_UI);

	//m_StartButton = static_cast<CStartButton*>(pButtonStart);
}

void CLobby::Update()
{
	CScene::Update();
}

void CLobby::Render(HDC _dc)
{
	Render_Map(_dc);
	HDC adDc = CBmpManager::Get_Instance()->Find_Image(L"shrroom");
	GdiTransparentBlt(_dc,
		100,
		200,
		887,
		221,
		adDc,
		0,
		0,
		887,	// 복사할 비트맵 가로 세로 사이즈
		221,
		RGB(1, 1, 1));	// 제거할 픽셀 색상 값

	auto str = CGameManager::GetInstance()->GetAccount();
	TextOut(MapDC,
		1140,
		25,
		str.c_str(),
		lstrlen(str.c_str()));

	CScene::Render(_dc);
}

void CLobby::Enter()
{
	Initialize();
}

void CLobby::Exit()
{
	DeleteAll();
}

void CLobby::Render_Map(HDC hdc)
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