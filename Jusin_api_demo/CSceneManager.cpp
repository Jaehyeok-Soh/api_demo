#include "pch.h"
#include "CSceneManager.h"
#include "CScrollManager.h"
CSceneManager* CSceneManager::m_pInstance = nullptr;

CSceneManager::CSceneManager() :m_arrStage{}, m_pCurScene(nullptr), m_pPlayer(nullptr), m_bChange(false)
{

}

CSceneManager::~CSceneManager()
{

	// 씬 전부 삭제
	for (UINT i = 0; i < SC_END; ++i)
	{
		if (nullptr != m_arrStage[i])
		{
			delete m_arrStage[i];
		}
	}

	//if (m_NextSceneNum != STAGE_TYPE::LOGO
	//	&& m_NextSceneNum != STAGE_TYPE::MENU)
	//{
	//	Safe_Delete(m_pPlayer); // 플레이어 삭제
	//	Safe_Delete(m_pWeapon);
	//}
}

void CSceneManager::Initialize()
{
	// Scene 생성
	/*m_arrStage[SC_LOADING] = new CLoading;
	m_arrStage[SC_LOADING]->SetName(L"LOADING");

	m_arrStage[SC_PLAY] = new CPlay;
	m_arrStage[SC_PLAY]->SetName(L"PLAY");*/

	// 현재 씬 지정
	m_CurSceneNum = SC_LOADING;

	m_pCurScene = m_arrStage[m_CurSceneNum];
	m_pCurScene->Enter();
}

void CSceneManager::Update()
{
	if (m_bChange)
		ChangeScene(m_NextSceneNum);

	m_pCurScene->Update();

	m_pCurScene->Late_Update();
}


void CSceneManager::Render(HDC _dc)
{
	m_pCurScene->Render(_dc);

}

void CSceneManager::ChangeScene(SCENEID _eNext)
{
	m_pCurScene->Exit();
	m_pCurScene = m_arrStage[(UINT)_eNext];

	CScrollManager::Get_Instance()->Reset_Scroll();
	m_pCurScene->Enter();

	m_bChange = false;
}