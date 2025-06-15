#include "pch.h"
#include "CMainGame.h"
#include "CTimeManager.h"
#include "CSceneManager.h"
#include "CBmpManager.h"
#include "CKeyManager.h"

CMainGame::CMainGame()
{
}

CMainGame::~CMainGame()
{
}

void CMainGame::Initialize()
{
	ClientCursorLock();
	
	m_DC = GetDC(g_hWnd);

	Load_Img();

	CTimeManager::Get_Instance()->Initialize();
	CSceneManager::GetInstance()->Initialize();
}

void CMainGame::Update()
{
	GetCursorPos(&g_ptMousePos);
	ScreenToClient(g_hWnd, &g_ptMousePos); // ȭ�� ��ǥ �� Ŭ���̾�Ʈ ��ǥ

	CTimeManager::Get_Instance()->Update();
	CSceneManager::GetInstance()->Update();
	CKeyManager::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
}

void CMainGame::Render()
{
	// 1. ���� ������ DC
	HDC hdc = m_DC;

	// 2. ����� DC ����
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hBackBmp = CreateCompatibleBitmap(hdc, WINCX, WINCY);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBackBmp);

	HDC hGroundDC = CBmpManager::Get_Instance()->Find_Image(L"Ground");
	BitBlt(hMemDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);

	// 4. �� �Ŵ����� ����ۿ� �׸����� ȣ��
	CSceneManager::GetInstance()->Render(hMemDC);

	// 5. ����� �� ���� ȭ������ ����
	BitBlt(hdc, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	/////////////////////////////////////////////////////////////////
	// pt.x, pt.y�� ���ڿ��� ��ȯ
	WCHAR szCursor[64];
	wsprintf(szCursor, L"Cursor: X = %d, Y = %d", g_ptMousePos.x, g_ptMousePos.y);

	// ��� (��: �»�� 10,10�� ���)
	TextOut(hdc, 10, 10, szCursor, lstrlen(szCursor));
	/////////////////////////////////////////////////////////////////

	// 6. �ڿ� ����
	SelectObject(hMemDC, hOldBmp);
	DeleteObject(hBackBmp);
	DeleteDC(hMemDC);

	CTimeManager::Get_Instance()->Render();
}

void CMainGame::Release()
{
	CTimeManager::Destroy_Instance();
	CSceneManager::DestroyInstance();
}

void CMainGame::ClientCursorLock()
{
	RECT rc{ 0, 0, WINCX, WINCY };
	POINT p1, p2;

	p1.x = rc.left;
	p1.y = rc.top;
	p2.x = rc.right;
	p2.y = rc.bottom;

	ClientToScreen(g_hWnd, &p1);
	ClientToScreen(g_hWnd, &p2);

	rc.left = p1.x;
	rc.top = p1.y - 50.f;
	rc.right = p2.x;
	rc.bottom = p2.y;

	ClipCursor(&rc);
}

void CMainGame::ClientCursorUnlock()
{
	ClipCursor(NULL);
}

void CMainGame::Load_Img()
{
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Ground.bmp", L"Ground");

	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/MapBig.bmp", L"MapBig");

	Load_CharacterImg();
}

void CMainGame::Load_CharacterImg()
{
	//Sword Man
	//Motion
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_idle_r.bmp", L"swordman_idle_r");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_run_r.bmp", L"swordman_run_r");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_attack_r.bmp", L"swordman_attack_r");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_ult_r.bmp", L"swordman_ult_r");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_die_r.bmp", L"swordman_die_r");
	//Effect
}

