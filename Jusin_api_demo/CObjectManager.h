#pragma once
#include "CObject.h"
#include "CPlayer.h"

class CObjectManager
{
public:
	CObjectManager();
	~CObjectManager();

	CObjectManager(CObjectManager&) = delete;
	CObjectManager(CObjectManager&&) = delete;
	CObjectManager& operator()(CObjectManager&) = delete;
	CObjectManager& operator()(CObjectManager&&) = delete;

public:
	static CObjectManager* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CObjectManager();
		}
		return m_pInstance;
	}

	static void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	template<typename T>
	CObject* CreateObject(OBJID _eId)
	{
		CObject* pObj = new T();

		return pObj;
	}

	void AddObject(OBJID _eId, CObject* _pObj)
	{
		m_ObjectList[_eId].push_back(_pObj);
	}

	CObject* GetPlayer()
	{
		find_if(m_ObjectList[OBJ_PLAYER].begin(), m_ObjectList[OBJ_PLAYER].end(), [](CObject* pObj) {
			if (dynamic_cast<CPlayer*>(pObj)->GetIsMine())
				return true;
			});
	}

private:
	static CObjectManager* m_pInstance;
	vector<CObject*> m_ObjectList[OBJ_END];
};

