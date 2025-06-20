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

void CCharacter::OnHit(int _iVal)
{
	m_tStatusInfo.m_iHp -= _iVal;

	if (m_tStatusInfo.m_iHp <= 0)
		Set_Dead();
}

