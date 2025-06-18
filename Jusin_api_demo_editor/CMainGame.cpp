#include "pch.h"
#include "CMainGame.h"
#include "CTimeManager.h"
#include "CSceneManager.h"
#include "CBmpManager.h"
#include "CKeyManager.h"
#include "CColliderManager.h"
#include "CPeekingManager.h"

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

	//백버퍼 DC 생성
	m_hBackDC = CreateCompatibleDC(m_DC);
	m_hBackBmp = CreateCompatibleBitmap(m_DC, WINCX, WINCY);
	m_hOldBmp = (HBITMAP)SelectObject(m_hBackDC, m_hBackBmp);

	Load_Img();

	CTimeManager::Get_Instance()->Initialize();
	CSceneManager::GetInstance()->Initialize();
}

void CMainGame::Update()
{
	if (CKeyManager::Get_Instance()->Key_Down('C'))
		bColRender = !bColRender;

	GetCursorPos(&g_ptMousePos);
	ScreenToClient(g_hWnd, &g_ptMousePos); // 화면 좌표 → 클라이언트 좌표

	CTimeManager::Get_Instance()->Update();
	CSceneManager::GetInstance()->Update();
	CKeyManager::Get_Instance()->Update();
	CColliderManager::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
}

void CMainGame::Render()
{
	HDC hGroundDC = CBmpManager::Get_Instance()->Find_Image(L"Ground");
	BitBlt(m_hBackDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);

	// 4. 씬 매니저가 백버퍼에 그리도록 호출
	CSceneManager::GetInstance()->Render(m_hBackDC);

	// 5. 백버퍼 → 실제 화면으로 복사
	BitBlt(m_DC, 0, 0, WINCX, WINCY, m_hBackDC, 0, 0, SRCCOPY);

	/////////////////////////////////////////////////////////////////
	// pt.x, pt.y를 문자열로 변환
	WCHAR szCursor[64];
	wsprintf(szCursor, L"Cursor: X = %d, Y = %d", g_ptMousePos.x, g_ptMousePos.y);

	// 출력 (예: 좌상단 10,10에 출력)
	TextOut(m_DC, 10, 10, szCursor, lstrlen(szCursor));
	/////////////////////////////////////////////////////////////////

	CTimeManager::Get_Instance()->Render();
}

void CMainGame::Release()
{
	if (m_hBackDC)
	{
		SelectObject(m_hBackDC, m_hOldBmp);
		DeleteObject(m_hBackBmp);
		DeleteDC(m_hBackDC);
	}

	CPeekingManager::DestroyInstance();
	CKeyManager::Destroy_Instance();
	CColliderManager::Destroy_Instance();
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

	//Map ARAM
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/MapBig.bmp", L"MapBig");
	
	Load_CharacterImg();
	Load_TowerImg();
}

void CMainGame::Load_CharacterImg()
{
	//Minion
	//13, 44 to 60, 27
	//181X41 //46
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/BlueMinion_r.bmp", L"BlueMinion_r");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/BlueMinion_l.bmp", L"BlueMinion_l");
	//96, 13 to 60, 27
	//184x41 //46
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/RedMinion_r.bmp", L"RedMinion_r");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/RedMinion_l.bmp", L"RedMinion_l");

	
	//Sword Man
	//Motion
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_idle_r.bmp", L"swordman_idle_r");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_run_r.bmp", L"swordman_run_r");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_attack_r.bmp", L"swordman_attack_r");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_skill_r.bmp", L"swordman_skill_r");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_ult_r.bmp", L"swordman_ult_r");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_die_r.bmp", L"swordman_die_r");
	//
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_idle_l.bmp", L"swordman_idle_l");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_run_l.bmp", L"swordman_run_l");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_attack_l.bmp", L"swordman_attack_l");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_skill_l.bmp", L"swordman_skill_l");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_ult_l.bmp", L"swordman_ult_l");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_die_l.bmp", L"swordman_die_l");
	//Effect
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_attack_ef_r.bmp", L"swordman_attack_ef_r");
	//CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_attack_ef_l.bmp", L"swordman_attack_ef_l");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_skill_ef_r.bmp", L"swordman_skill_ef_r");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_skill_ef_l.bmp", L"swordman_skill_ef_l");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_ult_ef_r.bmp", L"swordman_ult_ef_r");
	//CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Character/swordman/swordman_ult_ef_l.bmp", L"swordman_ult_ef_l");



}

void CMainGame::Load_TowerImg()
{
	//157x168
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Towers/BlueNexus.bmp", L"BlueNexus");
	//162x186
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Towers/BlueTurret.bmp", L"BlueTurret");
	//87x149
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Towers/BlueTurretSmall.bmp", L"BlueTurretSmall");
	//87x149
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Towers/BlueInhibitor.bmp", L"BlueInhibitor");
	//106x121
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Towers/BlueTurretSmall.bmp", L"BlueTurretSmall");

	//161x208
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Towers/RedNexus.bmp", L"RedNexus");
	//144x190
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Towers/RedTurret.bmp", L"RedTurret");
	//94x169
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Towers/RedTurretSmall.bmp", L"RedTurretSmall");
	//108x125
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Towers/RedInhibitor.bmp", L"RedInhibitor");
	//94x169
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/Towers/RedTurretSmall.bmp", L"RedTurretSmall");
}

