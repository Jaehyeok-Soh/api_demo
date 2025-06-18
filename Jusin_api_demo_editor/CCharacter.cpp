#include "pch.h"
#include "CCharacter.h"
#include "CPlayer.h"

CCharacter::CCharacter()
	: m_pWeapon(nullptr)
{
	ZeroMemory(&m_tAttackInfo, sizeof(ATTACKINFO));
	ZeroMemory(&m_tStatusInfo, sizeof(STATUSINFO));
}

CCharacter::~CCharacter()
{
}

void CCharacter::OnHit(ATTACKINFO _tAttackInfo)
{
	m_tStatusInfo.m_iHp -= _tAttackInfo.m_iDamage;
}

