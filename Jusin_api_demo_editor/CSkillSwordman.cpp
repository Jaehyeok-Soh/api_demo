#include "pch.h"
#include "CSkillSwordman.h"

CSkillSwordman::CSkillSwordman()
{
}

CSkillSwordman::~CSkillSwordman()
{
}

void CSkillSwordman::Update()
{
}

void CSkillSwordman::Initialize(CObject* _pObj)
{
}


void CSkillSwordman::Use(CPlayer& _pPlayer, int _iDir)
{
	_pPlayer.Set_FrameKey(L"swordmand_skill_" + (_iDir > 0) ? L"r" : L"l");
	_pPlayer.m_tAttackInfo.m_iDamage = 40;
}

bool CSkillSwordman::IsFinished(CPlayer& _pPlayer, int _iDir)
{
	return false;
}