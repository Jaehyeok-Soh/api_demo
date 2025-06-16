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

public:
	const TILETYPE GetDrawID() { return m_eDrawID; }
	void SetDrawID(TILETYPE _eVal) { m_eDrawID = _eVal; }

	const int GetOption() { return m_iOption; }
	void SetOption(int _iVal) { m_iOption = _iVal; }

	const bool GetTeam() const { return m_bTeam; }
	void SetTeam(bool _bVal) { m_bTeam = _bVal; }

protected:
	virtual void Motion_Change() {};

protected:
	TILETYPE m_eDrawID;
	int		m_iOption;
	
	ATTACKINFO m_tAttackInfo;
	int		m_iHP;


	//true 블루, false 레드
	//중립 없으니 그냥 bool
	bool	m_bTeam;
};

