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

	void SetPlayer(CObject* _pPlayer) { m_pPlayer = _pPlayer; }
	void SetWeapon(CObject* _pWeapon) { m_pWeapon = _pWeapon; }

	CObject* GetPlayer() { return m_pPlayer; }
	CObject* GetWeapon() { return m_pWeapon; }

	void SetChangeScene(bool bChange, SCENEID NextSceneNum) { m_bChange = bChange; m_NextSceneNum = NextSceneNum; }
	void ChangeScene(SCENEID _eNext);

	void AddOtherPlayer(CObject* _pObj) { m_otherPlayerList.push_back(_pObj); }
	list<CObject*> GetOtherPlayer() { return m_otherPlayerList; }

private:
	CScene* m_arrStage[SC_END]; // ��� �� ���
	CScene* m_pCurScene; // ���� ��
	CObject* m_pPlayer; // �÷��̾� ���� ������ �߰�
	list<CObject*> m_otherPlayerList; // �ٸ� �÷��̾� ����
	CObject* m_pWeapon;    //�÷��̾� ���� ����������

private:
	static CSceneManager* m_pInstance;
	SCENEID           m_CurSceneNum;      //���� �� ��ȣ(ENUM)

	bool                 m_bChange;
	SCENEID          m_NextSceneNum;
};

