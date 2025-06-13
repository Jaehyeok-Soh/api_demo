#include "pch.h"
#include "CMainGame.h"
#include "CTimeManager.h"
#include "CSceneManager.h"
#include "CBmpManager.h"

CMainGame::CMainGame()
{
}

CMainGame::~CMainGame()
{
}

void CMainGame::Initialize()
{
	m_DC = GetDC(g_hWnd);
	Load_Img();

	CTimeManager::Get_Instance()->Initialize();
	CSceneManager::GetInstance()->Initialize();
}

void CMainGame::Update()
{
	CTimeManager::Get_Instance()->Update();
	CSceneManager::GetInstance()->Update();
}

void CMainGame::Late_Update()
{
}

void CMainGame::Render()
{
	/*HDC	hBackDC = CBmpManager::Get_Instance()->Find_Image(L"Back");
	HDC	hGroundDC = CBmpManager::Get_Instance()->Find_Image(L"Ground");
	BitBlt(hBackDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);

	CSceneManager::GetInstance()->Render(m_DC);*/

	// 1. ���� ������ DC
	HDC hdc = m_DC;

	// 2. ����� DC ����
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hBackBmp = CreateCompatibleBitmap(hdc, WINCX, WINCY);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBackBmp);

	// 3. ����� �ʱ�ȭ (���)
	HDC hGroundDC = CBmpManager::Get_Instance()->Find_Image(L"Ground");
	BitBlt(hMemDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);

	// 4. �� �Ŵ����� ����ۿ� �׸����� ȣ��
	CSceneManager::GetInstance()->Render(hMemDC);

	// 5. ����� �� ���� ȭ������ ����
	BitBlt(hdc, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

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

void CMainGame::Load_Img()
{
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/Ground.bmp", L"Ground");

	CBmpManager::Get_Instance()->Insert_Bmp(L"../Image/ApiDemo/MapBig.bmp", L"MapBig");
}

void CMainGame::Load_PlayerImg()
{
}

