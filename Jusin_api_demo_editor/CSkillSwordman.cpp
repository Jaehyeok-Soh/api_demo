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
	if (_pPlayer.m_eCurState == CPlayer::SKILL)
	{
		Vec2 offset = Vec2(_pPlayer.m_vMoveDir.x * 10.f, 0.f);
		Vec2 pos = _pPlayer.GetPos() + offset;

		CHitbox* pHitbox = new CHitbox();
		pHitbox->SetPos(pos);
		pHitbox->SetScale(Vec2(32.f, 32.f));
		pHitbox->Initialize(CHitbox::HITBOXINFO{
			0.1f,		//duration
			0.f,		//elapsed
			40,			//damage
			false,		//once
			false,		//hitapplied
			&_pPlayer	//owner
			});

		if (m_bIsTrigger)
			return;

		m_bIsTrigger = true;

		CSceneManager::GetInstance()->GetCurScene()->AddObject(pHitbox, OBJ_HITBOX);

		CEffect* pEffect = new CEffect();
		FRAME tFrame;
		tFrame.iFrameStart = 0;
		tFrame.iFrameEnd = 15;
		tFrame.iMotion = 0;
		tFrame.iStartBuffer = 0;
		tFrame.dwSpeed = 100;
		tFrame.dwTime = GetTickCount();
		BMPSCALE tScale;
		tScale.iWidth = 76;
		tScale.iHeight = 54;
		pEffect->SetPos(pos);
		pEffect->SetScale(Vec2(38.f, 27.f));
		pEffect->Initialize(tFrame, tScale, L"swordman_skill_ef_r", RGB(255, 255, 255));
		CSceneManager::GetInstance()->GetCurScene()->AddObject(pEffect, OBJ_EFFECT);
	}

	if (_pPlayer.m_eCurState == CPlayer::SKILL && (_pPlayer.m_tFrame.iFrameStart == _pPlayer.m_tFrame.iFrameEnd)
		|| _pPlayer.m_eCurState != CPlayer::SKILL)
		m_bIsTrigger = false;
}

void CSkillSwordman::Initialize(CPlayer& _pPlayer)
{
}


void CSkillSwordman::Use(CPlayer& _pPlayer, int _iDir)
{
	_pPlayer.Set_FrameKey(L"swordman_skill_" + (_iDir > 0) ? L"r" : L"l");
	_pPlayer.SetState(CCharacter::SKILL);
}

bool CSkillSwordman::IsFinished(CPlayer& _pPlayer, int _iDir)
{
	return false;
}