#pragma once
#include "CComponent.h"
#include "CCollider.h"
#include "CObject.h"
#include <cassert>

class CComponentManager
{
public:
	CComponentManager();
	~CComponentManager();

public:
	static CComponentManager* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CComponentManager();
		}

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
	CComponentManager(CComponentManager&) = delete;
	CComponentManager(CComponentManager&&) = delete;
	CComponentManager& operator()(CComponentManager&) = delete;
	CComponentManager& operator()(CComponentManager&&) = delete;

public:
	template<typename T>
	CComponent* Add_Component(CObject* _pObj, COMPONENTID _eId)
	{
		CComponent* pComponent = new T();
		pComponent->Initialize(_pObj);
		m_pComponentList[_eId].push_back(pComponent);

		return pComponent;
	}

	template<>
	CComponent* Add_Component<CCollider>(CObject* _pObj, COMPONENTID _eId)
	{
		assert(false && "only Collider component");
		return nullptr;
	}

	CComponent* Add_Component(CObject* _pObj, COMPONENTID _eId, float _fCX, float _fCY, COLLAYERID _eLayer, int _iMask)
	{
		CCollider* pCollider = new CCollider();
		pCollider->Initialize(_pObj);
		pCollider->Set_Layer(_eLayer);
		pCollider->Set_Mask(_iMask);
		m_pComponentList[COMPONENT_COLLIDER].push_back(pCollider);

		return pCollider;
	}

public:
	void Update();
	void Update_All();
	void Late_Update();
	void Release();

	void Collider_Rend(HDC _hDC);

private:
	void Collision_Update();

private:
	static CComponentManager* m_pInstance;

	list<CComponent*> m_pComponentList[COMPONENT_END];
};