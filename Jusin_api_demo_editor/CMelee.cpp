#include "pch.h"
#include "CMelee.h"
#include "CTimeManager.h"
#include "CCharacter.h"

CMelee::CMelee()
{
}

CMelee::~CMelee()
{
}

int CMelee::Update()
{
	__super::Update();
    return NOEVENT;
}

void CMelee::Late_Update()
{
	__super::Late_Update();
}

void CMelee::Render(HDC _dc)
{
	//TODO: effect
}

void CMelee::Release()
{
}

void CMelee::Attack()
{
	m_tAttackInfo.m_fdtAttackTime += fDT;
	static_cast<CCharacter*>(m_pOwner->GetTarget())->OnHit(m_tAttackInfo.m_iDamage);
	m_tAttackInfo.m_bIsAttack = true;
}