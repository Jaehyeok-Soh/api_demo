#include "pch.h"
#include "CPeekingManager.h"
#include "CSceneManager.h"

CPeekingManager* CPeekingManager::m_pInstance = nullptr;

CPeekingManager::CPeekingManager()
	: m_pPlayer(nullptr)
{
}

CPeekingManager::~CPeekingManager()
{
}

void CPeekingManager::Initialize()
{
	m_pPlayer = CSceneManager::GetInstance()->GetCurScene()->GetPlayer();
}
