#pragma once
#include "CObject.h"

class CWeapon
	: public CObject
{
public:
	CWeapon();
	virtual ~CWeapon() PURE;

	virtual void Initialize() override;
	void Initialize(CObject* _pOwner, ATTACKINFO _tAttackInfo);
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC _dc) PURE;
	virtual void Release() PURE;

public:
	virtual void Attack();

protected:
	void AttackTime_Update();
	void OwnerPos_Update();

protected:
	CObject* m_pOwner;
	ATTACKINFO m_tAttackInfo;
};

