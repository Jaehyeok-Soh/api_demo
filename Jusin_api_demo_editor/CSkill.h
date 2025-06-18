#pragma once
#include "CPlayer.h"

class CSkill
{
public:
	CSkill();
	virtual ~CSkill() PURE;
	
public:
	virtual void Update(CPlayer& _pPlayer) PURE;
	virtual void Initialize(CPlayer& _pPlayer) PURE;
	virtual void Use(CPlayer& _pPlayer, int _iDir) PURE;
	virtual bool IsFinished(CPlayer& _pPlayer, int _iDir) PURE;

protected:
	SKILLINFO m_tSkill;
	bool m_bEnabled;
};

