#pragma once
#include "CObject.h"

class CCharacter
	: public CObject
{
public:
	CCharacter();
	virtual ~CCharacter() PURE;

protected:
	virtual void Motion_Change() {};

private:
	int		m_iHP;
};

