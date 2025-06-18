#pragma once
#include "CComponent.h"
#include "CObject.h"

class CCollider
	: public CComponent
{
	friend class CObject;

public:
	CCollider();
	CCollider(const CCollider& _origin);
	~CCollider();
	
	CCollider& operator = (CCollider* _origin) = delete;

public:
	void Initialize(CObject* _pObj);
	void Update();
	void Late_Update();
	void Render(HDC _dc);

public:
	void SetOwner(CObject* _pObj) { m_pOwner = _pObj; }

	void SetOffsetPos(Vec2 _vPos) { m_vOffsetPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	Vec2 GetOffsetPos() { return m_vOffsetPos; }
	Vec2 GetScale() { return m_vScale; }
	Vec2 GetFinalPos() { return m_vFinalPos; }
	UINT GetID() { return m_iID; }

	void Set_Layer(COLLAYERID _eLayer) { m_eLayer = _eLayer; }
	const COLLAYERID Get_Layer() const { return m_eLayer; }

	void Set_Mask(int _iMask) { m_iMask = _iMask; }
	const int Get_Mask() const { return m_iMask; }

public:
	// 충돌 시점 함수
	void OnCollision(CCollider* _pOther); // 충돌 중인 경우 호출되는 함수
	void OnCollisionEnter(CCollider* _pOther); // 충돌 진입 시
	void OnCollisionExit(CCollider* _pOther); // 충돌 해제 시

private:
	static UINT g_iNextID;

	Vec2 m_vOffsetPos; // 오브젝트로부터 상대적인 위치
	Vec2 m_vFinalPos; // finalupdate에서 매 프레임 마다 계산
	Vec2 m_vScale; // 충돌체의 크기

	UINT m_iID; // 충돌체 고유한 ID 값
	UINT m_iCol;

	COLLAYERID m_eLayer;
	int m_iMask;
};


