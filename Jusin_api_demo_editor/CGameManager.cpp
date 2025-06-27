#include "pch.h"
#include "CGameManager.h"
#include "CTcpManager.h"
#include "CPlayer.h"
#include "CSceneManager.h"
#include "CPlay.h"
CGameManager* CGameManager::m_pInstance = nullptr;

CGameManager::CGameManager()
	: m_fPlayTime(0.f)
{
}

CGameManager::~CGameManager()
{
}

void CGameManager::AddSequenceObject(TILETYPE _eVal, int _iOption)
{
	unordered_map<int, bool> pairBase;
	pairBase[_iOption] = false;
	m_mapGameSequence[_eVal] = pairBase;
}

void CGameManager::CheckEndOfGame()
{
	bool gameSet = false;
	bool team = false;
	//넥서스터지는거확인하고 승패판정전달
	if (CheckSequence(TILETYPE::BLUE_NEXUS, 1))
	{
		static_cast<CPlayer*>(CSceneManager::GetInstance()->GetPlayer())->ToDTO(true, false, "REDWIN");
		team = false;
		gameSet = true;
	}
	else if (CheckSequence(TILETYPE::RED_NEXUS, 1))
	{
		static_cast<CPlayer*>(CSceneManager::GetInstance()->GetPlayer())->ToDTO(true, false, "BLUEWIN");
		team = true;
		gameSet = true;
	}

	if (gameSet)
	{
		auto sceneManager = CSceneManager::GetInstance();
		auto curScene = static_cast<CPlay*>(sceneManager->GetCurScene());
		auto player = static_cast<CPlayer*>(sceneManager->GetPlayer());
		
		if (team)
		{
			if (player->GetTeam())
			{
				//플레이 씬 종료 연출 승
				curScene->GameSet(true);

			}
			else
			{
				//플레이 씬 종료 연출 패
				curScene->GameSet(false);
			}
		}
		else
		{
			if (player->GetTeam())
			{
				//플레이 씬 종료 연출 패
				curScene->GameSet(false);
			}
			else
			{
				//플레이 씬 종료 연출 승
				curScene->GameSet(true);
			}
		}
	}
}

void CGameManager::Update()
{
	AddPlayTime();
	CheckEndOfGame();
}

void CGameManager::Initialize()
{
}

void CGameManager::Release()
{
}
