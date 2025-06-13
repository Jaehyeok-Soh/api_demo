#include "pch.h"
#include "CComponent.h"
#include "CComponentManager.h"
#include "CCollisionManager.h"

CComponentManager* CComponentManager::m_pInstance;

CComponentManager::CComponentManager()
{
}

CComponentManager::~CComponentManager()
{
	Release();
}

void CComponentManager::Update()
{
	for (int i = 0; i < COMPONENT_END; i++)
	{
		for (auto pComponent : m_pComponentList[i])
		{
			pComponent->Update();
		}
	}
}

void CComponentManager::Update_All()
{

}

void CComponentManager::Late_Update()
{
	Collision_Update();
}

void CComponentManager::Collision_Update()
{
	//CCollisionManager::Collision_Rect_Resolve(m_pComponentList[COMPONENT_COLLIDER], m_pComponentList[COMPONENT_COLLIDER]);
}

void CComponentManager::Collider_Rend(HDC _hDC)
{
	for (auto pCollider : m_pComponentList[COMPONENT_COLLIDER])
		dynamic_cast<CCollider*>(pCollider)->Render(_hDC);
}

void CComponentManager::Release()
{
	for (int i = 0; i < COMPONENT_END; i++)
	{
		for_each(m_pComponentList[i].begin(), m_pComponentList[i].end(), DeleteObj<CComponent*>());
		m_pComponentList[i].clear();
	}
}

