#include "pch.h"
#include "CPlay.h"
#include "CTileManager.h"
#include "CScrollManager.h"
#include "CKeyManager.h"
#include "CBmpManager.h"
#include "CCollisionManager.h"
#include "CSceneManager.h"
#include "CObjectManager.h"

CPlay::CPlay()
{
}

CPlay::~CPlay()
{
}

void CPlay::Update()
{
	CTileManager::Get_Instance()->Update();

	Key_Input();

	//Late_Update
	CTileManager::Get_Instance()->Late_Update();
	CScene::Update();
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

	CObject* pPlayer = new CPlayer();
	pPlayer->Initialize();
	pPlayer->SetPos(Vec2(50.f, 800.f));
	AddObject(pPlayer, OBJ_PLAYER);
}

void CPlay::Key_Input()
{
	//if (CKeyManager::Get_Instance()->Key_Pressing(VK_LEFT))
	//	CScrollManager::Get_Instance()->Set_ScrollX(5.f);

	//if (CKeyManager::Get_Instance()->Key_Pressing(VK_RIGHT))
	//	CScrollManager::Get_Instance()->Set_ScrollX(-5.f);

	//if (CKeyManager::Get_Instance()->Key_Pressing(VK_UP))
	//	CScrollManager::Get_Instance()->Set_ScrollY(5.f);

	//if (CKeyManager::Get_Instance()->Key_Pressing(VK_DOWN))
	//	CScrollManager::Get_Instance()->Set_ScrollY(-5.f);

	if (g_ptMousePos.x <= 10)
		CScrollManager::Get_Instance()->Set_ScrollX(5.f);

	if (g_ptMousePos.x >= WINCX - 10)
		CScrollManager::Get_Instance()->Set_ScrollX(-5.f);

	if (g_ptMousePos.y <= 10)
		CScrollManager::Get_Instance()->Set_ScrollY(5.f);

	if (g_ptMousePos.y >= WINCY - 10)
		CScrollManager::Get_Instance()->Set_ScrollY(-5.f);
}

void CPlay::Render_Map(HDC hdc, int iScrollX, int iScrollY)
{
	int iDestW = (int)(1920 * g_fZoom);
	int iDestH = (int)(1080 * g_fZoom);

	GdiTransparentBlt(hdc,
		(int)(iScrollX * g_fZoom),
		(int)(iScrollY * g_fZoom),
		iDestW,
		iDestH,
		MapDC,
		0,
		0,
		(int)1920,	// 복사할 비트맵 가로 세로 사이즈
		(int)1080,
		RGB(255, 0, 255));	// 제거할 픽셀 색상 값
}
