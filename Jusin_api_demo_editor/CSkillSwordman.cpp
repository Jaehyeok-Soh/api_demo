#include "pch.h"
#include "CSkillSwordman.h"
#include "CHitBox.h"
#include "CSceneManager.h"

CSkillSwordman::CSkillSwordman()
{
}

CSkillSwordman::~CSkillSwordman()
{
}

void CSkillSwordman::Update(CPlayer& _pPlayer)
{
}

void CSkillSwordman::Initialize(CPlayer& _pPlayer)
{
}


void CSkillSwordman::Use(CPlayer& _pPlayer, int _iDir)
{
	_pPlayer.Set_FrameKey(L"swordman_skill_" + (_iDir > 0) ? L"r" : L"l");
	_pPlayer.SetState(CCharacter::SKILL);

	Vec2 offset = Vec2(_iDir * 10.f, 0.f);
	Vec2 pos = _pPlayer.GetPos() + offset;

	CHitbox* pHitbox = new CHitbox();
	pHitbox->Initialize(CHitbox::HITBOXINFO{
		0.15f,		//duration
		0.f,		//elapsed
		40,			//damage
		true,		//once
		false,		//hitapplied
		&_pPlayer	//owner
		});
	pHitbox->SetPos(pos);
	pHitbox->SetScale(Vec2(16.f, 16.f));

	CSceneManager::GetInstance()->GetCurScene()->AddObject(pHitbox, OBJ_HITBOX);
}

bool CSkillSwordman::IsFinished(CPlayer& _pPlayer, int _iDir)
{
	return false;
}