#include "pch.h"
#include "CMainGame.h"
#include "CTimeManager.h"
#include "CSceneManager.h"

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
	CTimeManager::Get_Instance()->Render();
	CSceneManager::GetInstance()->Render(m_DC);
}

void CMainGame::Release()
{
	CTimeManager::Destroy_Instance();
	CSceneManager::DestroyInstance();
}
