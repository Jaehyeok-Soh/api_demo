#include "pch.h"
#include "CGameManager.h"
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

void CGameManager::Update()
{
	AddPlayTime();
}

void CGameManager::Initialize()
{
}

void CGameManager::Release()
{
}
