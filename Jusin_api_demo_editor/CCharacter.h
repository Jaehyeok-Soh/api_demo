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

protected:
	TILETYPE m_eDrawID;
	int		m_iOption;
	
	ATTACKINFO m_tAttackInfo;
	int		m_iHP;


	//true ���, false ����
	bool	m_bTeam;
};

