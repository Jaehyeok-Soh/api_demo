#pragma once
#include "CObject.h"
#include "CTile.h"

class CCharacter
	: public CObject
{
public:
	enum STATE { IDLE, RUN, ATTACK, SKILL, ULT, DIE, END };
public:
	CCharacter();
	virtual ~CCharacter() PURE;

protected:
	virtual void Motion_Change() {};

public:
	void OnHit(ATTACKINFO _tAttackInfo);

protected:
	ATTACKINFO m_tAttackInfo;
	int		m_iHP;
};

