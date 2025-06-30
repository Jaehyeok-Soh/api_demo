#pragma once
#include "CScene.h"
#include "CObject.h"

class CSceneManager
{
public:
	CSceneManager();
	~CSceneManager();

	CSceneManager(CSceneManager&) = delete;
	CSceneManager(CSceneManager&&) = delete;
	CSceneManager& operator()(CSceneManager&) = delete;
	CSceneManager& operator()(CSceneManager&&) = delete;

public:
	static CSceneManager* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CSceneManager();
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
	void Initialize();
	void Update();
	void Render(HDC _dc);

public:
	CScene* GetCurScene() { return m_pCurScene; }

	const SCENEID GetCurSceneNum() const { return m_CurSceneNum; }

	void SetPlayer(CObject* _pPlayer) { m_pPlayer = _pPlayer; }
	void SetWeapon(CObject* _pWeapon) { m_pWeapon = _pWeapon; }

	CObject* GetPlayer() { return m_pPlayer; }
	CObject* GetWeapon() { return m_pWeapon; }

	void ReSetScene() { m_bReSet = true; }

	void SetChangeScene(bool bChange, SCENEID NextSceneNum) { m_bChange = bChange; m_NextSceneNum = NextSceneNum; }
	void ChangeScene(SCENEID _eNext);

	void AddOtherPlayer(CObject* _pObj) { m_otherPlayerList.push_back(_pObj); }
	list<CObject*> GetOtherPlayer() { return m_otherPlayerList; }

	void AddOtherWeapon(CObject* _pObj) { m_otherWeaponList.push_back(_pObj); }
	list<CObject*> GetOtherWeapon() { return m_otherWeaponList; }

private:
	CScene* m_arrStage[SC_END]; // 모든 씬 목록
	CScene* m_pCurScene; // 현재 씬
	CObject* m_pPlayer; // 플레이어 공유 포인터 추가
	list<CObject*> m_otherPlayerList; // 다른 플레이어 정보
	CObject* m_pWeapon;    //플레이어 웨폰 공유포인터
	list<CObject*> m_otherWeaponList; // 다른 플레이어 웨펀 정보

private:
	static CSceneManager* m_pInstance;
	SCENEID           m_CurSceneNum;      //현재 씬 번호(ENUM)

	bool                 m_bChange;
	SCENEID          m_NextSceneNum;

	bool				m_bReSet;
};

