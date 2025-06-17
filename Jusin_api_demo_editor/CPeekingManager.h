#pragma once
#include "CObject.h"
#include "CPlayer.h"

class CPeekingManager
{
public:
	CPeekingManager();
	~CPeekingManager();

	CPeekingManager(CPeekingManager&) = delete;
	CPeekingManager(CPeekingManager&&) = delete;
	CPeekingManager& operator()(CPeekingManager&) = delete;
	CPeekingManager& operator()(CPeekingManager&&) = delete;

public:
	void Initialize();

public:
	static CPeekingManager* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CPeekingManager();
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
	void OnPeek(CObject* _pObj)
	{
		static_cast<CPlayer*>(m_pPlayer)->OnPeek(_pObj);
	}

private:
	static CPeekingManager* m_pInstance;
	CObject* m_pPlayer;
};

