#include "pch.h"
#include "CUltSwordman.h"
#include "CHitBox.h"
#include "CSceneManager.h"
#include "CEffect.h"
#include "CTimeManager.h"
#include "CTileManager.h"

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
		CHitbox* pHitbox = new CHitbox();
		pHitbox->SetPosX(_pPlayer.m_vPos.x + 10.f * ((_pPlayer.m_vMoveDir.x > 0) ? 1.f : -1.f));
		pHitbox->SetPosY(_pPlayer.m_vPos.y);
		pHitbox->SetScale(Vec2(70.f, 50.f));
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

		auto tileManager = CTileManager::Get_Instance();
		float dist = 100.f;
		while (tileManager->CheckPeekDisable((_pPlayer.m_vPos.x + dist * ((_pPlayer.m_vMoveDir.x > 0) ? 1.f : -1.f)) / TILECX, _pPlayer.m_vPos.y / TILECY))
		{
			dist -= 4.f;
			
			if (dist == 0.f)
				break;
		}
		_pPlayer.m_vPos.x = _pPlayer.m_vPos.x + dist * ((_pPlayer.m_vMoveDir.x > 0) ? 1.f : -1.f);

		CSceneManager::GetInstance()->GetCurScene()->AddObject(pHitbox, OBJ_HITBOX);

		CEffect* pEffect = new CEffect();
		FRAME tFrame;
		tFrame.iFrameStart = 0;
		tFrame.iFrameEnd = 6;
		tFrame.iMotion = 0;
		tFrame.iStartBuffer = 0;
		tFrame.dwSpeed = 100;
		tFrame.dwTime = GetTickCount();
		BMPSCALE tScale;
		tScale.iWidth = 384;
		tScale.iHeight = 384;
		pEffect->SetPosX(_pPlayer.m_vPos.x + 30.f * ((_pPlayer.m_vMoveDir.x > 0) ? -1.f : 1.f));
		pEffect->SetPosY(_pPlayer.m_vPos.y);
		pEffect->SetScale(Vec2(70.f, 50.f));
		pEffect->Initialize(tFrame, tScale, L"swordman_ult_ef_r", RGB(255, 255, 255));
		CSceneManager::GetInstance()->GetCurScene()->AddObject(pEffect, OBJ_EFFECT);
	}

	if (_pPlayer.m_eCurState == CPlayer::ULT && (_pPlayer.m_tFrame.iFrameStart == _pPlayer.m_tFrame.iFrameEnd)
		|| _pPlayer.m_eCurState != CPlayer::ULT)
		m_bIsTrigger = false;
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
