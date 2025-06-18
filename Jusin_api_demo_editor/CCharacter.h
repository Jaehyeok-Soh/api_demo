#pragma once
#include "CObject.h"
#include "CTile.h"
#include "CWeapon.h"

class CCharacter
	: public CObject
{
public:
	enum STATE { IDLE, RUN, ATTACK, SKILL, ULT, DIE, END };
public:
	CCharacter();
	virtual ~CCharacter() PURE;

public:
	void OnHit(int _iVal);

protected:
	virtual void Motion_Change() {};
	virtual void CreateWeapon() PURE;

protected:
	ATTACKINFO m_tAttackInfo;
	STATUSINFO m_tStatusInfo;
	CWeapon* m_pWeapon;
};

