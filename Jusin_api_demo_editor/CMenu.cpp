#include "pch.h"
#include "CMenu.h"
#include "CBmpManager.h"
#include "CCollisionManager.h"
#include "CSceneManager.h"
#include "CButton.h"

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
	HDC	hMemDC = CBmpManager::Get_Instance()->Find_Image(L"MenuBackGround");

	BitBlt(_dc, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CScene::Render(_dc);
}

void CMenu::Enter()
{
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Button/Start.bmp", L"ButtonPlay");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Button/Edit.bmp", L"ButtonEdit");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Button/Exit.bmp", L"ButtonExit");

	Initialize();
}

void CMenu::Exit()
{
	DeleteAll();
	CCollisionManager::Get_Instance()->Reset();
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