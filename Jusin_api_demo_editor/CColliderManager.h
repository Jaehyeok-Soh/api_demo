#pragma once

union COLIDER_ID
{
	struct
	{
		UINT Left_Id;
		UINT Right_Id;
	};
	ULONGLONG ID;
};

class CComponent;
class CCollider;
class CColliderManager
{
public:
	CColliderManager();
	~CColliderManager();

public:
	void Update();
	void CheckGroup(OBJID _eLeft, OBJID _eRight);
	void Reset()
	{
		memset(m_arrCheck, 0, sizeof(UINT) * (UINT)OBJID::OBJ_END);
	}

private:
	void CollisionUpdateGroup(OBJID _eLeft, OBJID _eRight);
	bool IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol);

public:
	static CColliderManager* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CColliderManager;
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

private:
	static CColliderManager* m_pInstance;

	map<ULONGLONG, bool> m_mapColInfo;		// 충돌체 간의 이전 프레임 충돌 정보
	UINT m_arrCheck[(UINT)OBJID::OBJ_END];		// 그룹간의 충돌 체크 매트릭스
};