#pragma once
#include "CObject.h"

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

protected:
	int		m_iHP;
};

