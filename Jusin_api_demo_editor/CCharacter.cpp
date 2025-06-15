#include "pch.h"
#include "CCharacter.h"

CCharacter::CCharacter()
	: m_iHP(10),
	m_eDrawID(TILETYPE::BLUE_INHIBITER),
	m_iOption(1),
	m_bTeam(true)
{
	ZeroMemory(&m_tAttackInfo, sizeof(ATTACKINFO));
}

CCharacter::~CCharacter()
{
}
