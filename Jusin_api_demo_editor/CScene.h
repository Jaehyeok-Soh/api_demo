#pragma once

class CObject;
class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	CObject* GetPlayer() { return m_pPlayer; }
	//CObject* GetWeapon() { return m_pWeapon; }
	list<CObject*>* GetObjectList();

	void RegisterPlayer(CObject* _pPlayer) { m_pPlayer = _pPlayer; }
	//void RegisterWeapon(CObject* _pWeapon) { m_pWeapon = _pWeapon; }

	virtual void Initialize();
	virtual void Update();
	virtual void Late_Update();
	virtual void Render(HDC _dc);

	virtual void Enter() = 0; // 해당 Scene에 진입 시 호출
	virtual void Exit() = 0; // 해당 Scene을 탈출 시 호출

public:
	void AddObject(CObject* _pObj, OBJID _eType)
	{
		m_arrObj[(UINT)_eType].push_back(_pObj);
	}
	const list<CObject*>& GetGroupObject(OBJID _eType) { return m_arrObj[(UINT)_eType]; }
	void DeleteGroup(OBJID _eTarget);
	void DeleteAll();

protected:
	void		SetMaxScroll(const TCHAR* pImgKey);

protected:
	list<CObject*> m_arrObj[(UINT)OBJID::OBJ_END]; // 오브젝트를 저장 및 관리할 벡터를 그룹 개수만큼 선언
	wstring m_strName; // Scene이름

	CObject* m_pPlayer;		// Player

protected:
	HDC	MapDC;
	bool	m_bFade;
};

