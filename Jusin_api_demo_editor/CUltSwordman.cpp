#include "pch.h"
#include "CUltSwordman.h"
#include "CHitBox.h"
#include "CSceneManager.h"
#include "CEffect.h"

CUltSwordman::CUltSwordman()
{
}

CUltSwordman::~CUltSwordman()
{
}

void CUltSwordman::Update(CPlayer& _pPlayer)
{
	if (_pPlayer.m_eCurState == CPlayer::ULT && _pPlayer.m_tFrame.iFrameStart == 3)
	{
		_pPlayer.m_vPos.x = _pPlayer.m_vPos.x + 50.f * ((_pPlayer.m_vMoveDir.x > 0) ? 1.f : -1.f);

		CHitbox* pHitbox = new CHitbox();
		pHitbox->SetPos(_pPlayer.m_vPos);
		pHitbox->SetScale(Vec2(70.f, 50.f));
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
		tFrame.iFrameEnd = 6;
		tFrame.iMotion = 0;
		tFrame.iStartBuffer = 0;
		tFrame.dwSpeed = 200;
		tFrame.dwTime = GetTickCount();
		BMPSCALE tScale;
		tScale.iWidth = 384;
		tScale.iHeight = 384;
		pEffect->SetPos(_pPlayer.m_vPos);
		pEffect->SetScale(Vec2(70.f, 50.f));
		pEffect->Initialize(tFrame, tScale, L"swordman_ult_ef_r");
		CSceneManager::GetInstance()->GetCurScene()->AddObject(pEffect, OBJ_EFFECT);
	}
}

void CUltSwordman::Initialize(CPlayer& _pPlayer)
{
}

void CUltSwordman::Use(CPlayer& _pPlayer, int _iDir)
{
	_pPlayer.Set_FrameKey(L"swordman_ult_" + (_iDir > 0) ? L"r" : L"l");
	_pPlayer.SetState(CCharacter::ULT);
}

bool CUltSwordman::IsFinished(CPlayer& _pPlayer, int _iDir)
{
	return false;
}
