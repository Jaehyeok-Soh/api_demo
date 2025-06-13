#include "pch.h"
#include "CEdit.h"
#include "CSceneManager.h"
#include "CBmpManager.h"
#include "CCollisionManager.h"
#include "CTileManager.h"
#include "CKeyManager.h"
#include "CScrollManager.h"

CEdit::CEdit()
{
}

CEdit::~CEdit()
{
}

void CEdit::Update()
{
	CScene::Update();
	CTileManager::Get_Instance()->Update();

	Key_Input();

	//Late_Update
	CTileManager::Get_Instance()->Late_Update();
}

void CEdit::Render(HDC _dc)
{
	CScene::Render(_dc);
	CTileManager::Get_Instance()->Render(_dc);
}

void CEdit::Enter()
{
	Initialize();
}

void CEdit::Exit()
{
	DeleteAll();
	CCollisionManager::Get_Instance()->Reset();
}

void CEdit::Initialize()
{
	CSceneManager::GetInstance()->SetChangeScene(false, SC_EDIT);

	CTileManager::Get_Instance()->Initialize();
}

void CEdit::Key_Input()
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
