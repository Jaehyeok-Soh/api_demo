#include "pch.h"
#include "CMenu.h"
#include "CBmpManager.h"
#include "CColliderManager.h"
#include "CSceneManager.h"
#include "CButton.h"
#include "CScrollManager.h"

CMenu::CMenu()
{
}

CMenu::~CMenu()
{
}

void CMenu::Update()
{
	CScene::Update();
}

void CMenu::Render(HDC _dc)
{
	int iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = CScrollManager::Get_Instance()->Get_ScrollY();

	Render_Map(_dc, iScrollX, iScrollY);

	CScene::Render(_dc);
}

void CMenu::Enter()
{
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Button/Start.bmp", L"ButtonPlay");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Button/Edit.bmp", L"ButtonEdit");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Button/Exit.bmp", L"ButtonExit");

	MapDC = CBmpManager::Get_Instance()->Find_Image(L"MapBig");
	SetMaxScroll(L"MapBig");

	Initialize();
}

void CMenu::Exit()
{
	DeleteAll();
	CColliderManager::Get_Instance()->Reset();
}

void CMenu::Initialize()
{
	CSceneManager::GetInstance()->SetChangeScene(false, SC_MENU);

	CObject* pButtonStart = new CButton();
	pButtonStart->Initialize();
	pButtonStart->SetPos(Vec2(300.f, 300.f));
	pButtonStart->SetName(L"UI");
	pButtonStart->Set_FrameKey(L"ButtonPlay");
	AddObject(pButtonStart, OBJ_UI);

	CObject* pButtonEdit = new CButton();
	pButtonEdit->Initialize();
	pButtonEdit->SetPos(Vec2(500.f, 300.f));
	pButtonEdit->SetName(L"UI");
	pButtonEdit->Set_FrameKey(L"ButtonEdit");
	AddObject(pButtonEdit, OBJ_UI);

	CObject* pButtonExit = new CButton();
	pButtonExit->Initialize();
	pButtonExit->SetPos(Vec2(700.f, 300.f));
	pButtonExit->SetName(L"UI");
	pButtonExit->Set_FrameKey(L"ButtonExit");
	AddObject(pButtonExit, OBJ_UI);
}

void CMenu::Render_Map(HDC hdc, int iScrollX, int iScrollY)
{
	int iDestW = (int)(720 * g_fZoom);
	int iDestH = (int)(405 * g_fZoom);

	GdiTransparentBlt(hdc,
		(int)(iScrollX * g_fZoom),
		(int)(iScrollY * g_fZoom),
		iDestW,
		iDestH,
		MapDC,
		0,
		0,
		720,	// 복사할 비트맵 가로 세로 사이즈
		405,
		RGB(255, 0, 255));	// 제거할 픽셀 색상 값
}