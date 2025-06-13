#include "pch.h"
#include "CPlay.h"
#include "CTileManager.h"
#include "CScrollManager.h"
#include "CKeyManager.h"
#include "CBmpManager.h"
#include "CCollisionManager.h"
#include "CSceneManager.h"

CPlay::CPlay()
{
}

CPlay::~CPlay()
{
}

void CPlay::Update()
{
	CScene::Update();
	CTileManager::Get_Instance()->Update();

	Key_Input();

	//Late_Update
	CTileManager::Get_Instance()->Late_Update();
}

void CPlay::Render(HDC _dc)
{
	int iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = CScrollManager::Get_Instance()->Get_ScrollY();

	Render_Map(_dc, iScrollX, iScrollY);

	CTileManager::Get_Instance()->Render(_dc);

	CScene::Render(_dc);
}

void CPlay::Enter()
{
	MapDC = CBmpManager::Get_Instance()->Find_Image(L"MapBig");
	SetMaxScroll(L"MapBig");

	Initialize();
}

void CPlay::Exit()
{
	DeleteAll();
	CCollisionManager::Get_Instance()->Reset();
}

void CPlay::Initialize()
{
	CSceneManager::GetInstance()->SetChangeScene(false, SC_EDIT);

	CTileManager::Get_Instance()->Initialize();

	CTileManager::Get_Instance()->Load_Tile();
}

void CPlay::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollManager::Get_Instance()->Set_ScrollX(5.f);

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollManager::Get_Instance()->Set_ScrollX(-5.f);

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_UP))
		CScrollManager::Get_Instance()->Set_ScrollY(5.f);

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_DOWN))
		CScrollManager::Get_Instance()->Set_ScrollY(-5.f);

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		POINT ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		ptMouse.x -= (int)CScrollManager::Get_Instance()->Get_ScrollX();
		ptMouse.y -= (int)CScrollManager::Get_Instance()->Get_ScrollY();

		CTileManager::Get_Instance()->Picking_Tile(ptMouse, 1, 1);
	}
}

void CPlay::Render_Map(HDC hdc, int iScrollX, int iScrollY)
{
	GdiTransparentBlt(hdc,
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
