#pragma once
#include "CCollider.h"

class CObject
{
public:
	CObject();
	CObject(const CObject& _origin);
	virtual ~CObject() PURE;

public:
	virtual void	Initialize()	PURE;
	virtual int		Update()		PURE;
	virtual void	Late_Update()	PURE;
	virtual void	Render(HDC _dc)	PURE;
	virtual void	Release()		PURE;

public:
	const wstring& GetName() { return m_strName; }
	void SetName(const wstring& _strName) { m_strName = _strName; }
	void SetPos(Vec2  _vPos) { m_vPos = _vPos; }
	void SetPosX(float _f) { m_vPos.x = _f; }
	void SetPosY(float _f) { m_vPos.y = _f; }
	Vec2 GetPos() { return m_vPos; }

	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	Vec2 GetScale() { return m_vScale; }

	const Vec2 GetDir() const { return m_vMoveDir; }


	void		Set_Dead() { m_bDead = true; }
	void		Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	void		Set_Target(CObject* pTarget) { m_pTarget = pTarget; }
	void		Set_Dir(Vec2 _vDir) { m_vMoveDir = _vDir; }
	void		Set_Velocity(Vec2 _v) { m_vVelocity = _v; }

	Vec2 Get_Velocity() const { return m_vVelocity; }
	const RECT& Get_Rect()const { return m_tRect; }
	bool		Get_Dead() const { return m_bDead; }


	void		Set_FrameKey(const TCHAR* pFrameKey) { m_pFrameKey = pFrameKey; }

protected:
	void		Update_Rect();
	void		Update_Frame();

public:
	void CreateCollider();
	CCollider* GetCollider() { return m_pCollider; }
	//CGravity* GetGravity() { return m_pGravity; }
	void Component_Render(HDC _dc);

public:
	virtual void OnCollision(CCollider* _pOther) {}
	virtual void OnCollisionEnter(CCollider* _pOther) {}
	virtual void OnCollisionExit(CCollider* _pOther) {}

protected:
	wstring m_strName; // 이름
	CObject* m_pTarget; // 타겟
	Vec2 m_vPos; // 위치 좌표
	Vec2 m_vScale; // 크기
	Vec2 m_vMoveDir; // 이동할 방향
	Vec2 m_vVelocity; // 힘
	RECT m_tRect;
	INFO m_tInfo;

	float		m_fSpeed;
	float		m_fDistance;
	float		m_fAngle;
	bool		m_bDead;

	// Component
	CCollider* m_pCollider; // 콜라이더(충돌체)
	//CGravity* m_pGravity; // 중력

protected:
	const		TCHAR* m_pFrameKey;
	FRAME		m_tFrame;
};