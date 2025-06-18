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
	// �浹 ���� �Լ�
	void OnCollision(CCollider* _pOther); // �浹 ���� ��� ȣ��Ǵ� �Լ�
	void OnCollisionEnter(CCollider* _pOther); // �浹 ���� ��
	void OnCollisionExit(CCollider* _pOther); // �浹 ���� ��

private:
	static UINT g_iNextID;

	Vec2 m_vOffsetPos; // ������Ʈ�κ��� ������� ��ġ
	Vec2 m_vFinalPos; // finalupdate���� �� ������ ���� ���
	Vec2 m_vScale; // �浹ü�� ũ��

	UINT m_iID; // �浹ü ������ ID ��
	UINT m_iCol;

	COLLAYERID m_eLayer;
	int m_iMask;
};


