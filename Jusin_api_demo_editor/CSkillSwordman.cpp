#include "pch.h"
#include "CSkillSwordman.h"
#include "CHitBox.h"
#include "CEffect.h"
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
	pHitbox->SetPos(pos);
	pHitbox->SetScale(Vec2(32.f, 32.f));
	pHitbox->Initialize(CHitbox::HITBOXINFO{
		0.5f,		//duration
		0.f,		//elapsed
		40,			//damage
		false,		//once
		false,		//hitapplied
		&_pPlayer	//owner
		});

	CSceneManager::GetInstance()->GetCurScene()->AddObject(pHitbox, OBJ_HITBOX);

	CEffect* pEffect = new CEffect();
	FRAME tFrame;
	tFrame.iFrameStart = 0;
	tFrame.iFrameEnd = 15;
	tFrame.iMotion = 0;
	tFrame.iStartBuffer = 0;
	tFrame.dwSpeed = 20;
	tFrame.dwTime = GetTickCount();
	BMPSCALE tScale;
	tScale.iWidth = 76;
	tScale.iHeight = 54;
	pEffect->SetPos(pos);
	pEffect->SetScale(Vec2(38.f, 27.f));
	pEffect->Initialize(tFrame, tScale, L"swordman_skill_ef_r");
	CSceneManager::GetInstance()->GetCurScene()->AddObject(pEffect, OBJ_EFFECT);
}

bool CSkillSwordman::IsFinished(CPlayer& _pPlayer, int _iDir)
{
	return false;
}