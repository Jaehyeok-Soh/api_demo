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

	const bool	GetIsHide() const { return m_bIsHide; }
	void		SetIsHide(bool _bVal) { m_bIsHide = _bVal; }

	const int	GetHideOption() const { return m_iHideOption; }
	void		SetHideOption(int _iVal) { m_iHideOption = _iVal; }

protected:
	virtual void Motion_Change() {};
	virtual void CreateWeapon() PURE;

protected:
	ATTACKINFO m_tAttackInfo;
	STATUSINFO m_tStatusInfo;
	CWeapon* m_pWeapon;

	bool				m_bIsHide;
	int					m_iHideOption;
};

