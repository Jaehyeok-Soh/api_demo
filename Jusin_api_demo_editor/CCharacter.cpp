#include "pch.h"
#include "CCharacter.h"

CCharacter::CCharacter()
	: m_iHP(10)
{
	ZeroMemory(&m_tAttackInfo, sizeof(ATTACKINFO));
}

CCharacter::~CCharacter()
{
}

void CCharacter::OnHit(ATTACKINFO _tAttackInfo)
{
	m_iHP -= _tAttackInfo.m_iDamage;
}
