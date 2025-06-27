#pragma once
#include "CTimeManager.h"

class CGameManager
{
public:
	CGameManager();
	~CGameManager();

	CGameManager(CGameManager&) = delete;
	CGameManager(CGameManager&&) = delete;
	CGameManager& operator()(CGameManager&) = delete;
	CGameManager& operator()(CGameManager&&) = delete;

public:
	static CGameManager* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CGameManager();
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
	const float GetPlayTime() const { return m_fPlayTime; }
	void SetPlayTime(float _fPlayTime) { m_fPlayTime = _fPlayTime; }
	void AddPlayTime() { m_fPlayTime += fDT; }

	void AddSequenceObject(TILETYPE _eVal, int _iOption);
	const bool CheckSequence(TILETYPE _eVal, int _iOption) const { return (*(*m_mapGameSequence.find(_eVal)).second.find(_iOption)).second; }
	void SetSequence(TILETYPE _eVal, int _iOption) { (*(*m_mapGameSequence.find(_eVal)).second.find(_iOption)).second = true; }
	const pair<TILETYPE, unordered_map<int, bool>> GetGameSeqPair(TILETYPE _eVal, int _iOption) const { return (*m_mapGameSequence.find(_eVal)); }

	void SetAccount(wstring _wstrVal)
	{
		m_wstrAccount = _wstrVal;
		auto a = 1;
	}
	const wstring GetAccount() const { return m_wstrAccount; }

public:
	void CheckEndOfGame();

public:
	void Update();
	void Initialize();
	void Release();

private:
	static CGameManager* m_pInstance;
	float m_fPlayTime;
	unordered_map<TILETYPE, unordered_map<int, bool>> m_mapGameSequence;
	wstring m_wstrAccount;
};

