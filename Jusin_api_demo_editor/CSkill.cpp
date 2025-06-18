#include "pch.h"
#include "CSkill.h"
#include "CSkillSwordman.h"

CSkill::CSkill()
	: m_bEnabled(false)
{
	ZeroMemory(&m_tSkill, sizeof(SKILLINFO));
}

CSkill::~CSkill()
{
}
