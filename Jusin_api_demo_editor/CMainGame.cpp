#include "pch.h"
#include "CMainGame.h"
#include "CTimeManager.h"

CMainGame::CMainGame()
{
}

CMainGame::~CMainGame()
{
}

void CMainGame::Initialize()
{
	m_DC = GetDC(g_hWnd);

	CTimeManager::Get_Instance()->Initialize();
}

void CMainGame::Update()
{
	CTimeManager::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
}

void CMainGame::Render()
{
	CTimeManager::Get_Instance()->Render();
}

void CMainGame::Release()
{
	CTimeManager::Destroy_Instance();
}
