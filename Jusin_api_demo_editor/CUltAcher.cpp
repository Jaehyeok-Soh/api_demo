#include "pch.h"
#include "CUltAcher.h"
#include "CHitBox.h"
#include "CSceneManager.h"
#include "CEffect.h"
#include "CBullet.h"

void CUltAcher::Update(CPlayer& _pPlayer)
{
	if (_pPlayer.m_eCurState == CPlayer::ULT)
	{
		_pPlayer.FindTarget();

		CObject* pBullet = new CBullet();
		static_cast<CBullet*>(pBullet)->Initialize(_pPlayer.GetTarget());
		pBullet->SetPos(_pPlayer.GetPos());
		pBullet->SetScale(Vec2(24.f, 24.f));
		static_cast<CBullet*>(pBullet)->SetBulletWeapon(_pPlayer.m_pWeapon);
		CSceneManager::GetInstance()->GetCurScene()->AddObject(pBullet, OBJ_ATTACK);

		if (m_bIsTrigger)
			return;

		m_bIsTrigger = true;

		CEffect* pEffect = new CEffect();
		FRAME tFrame;
		tFrame.iFrameStart = 0;
		tFrame.iFrameEnd = 8;
		tFrame.iMotion = 0;
		tFrame.iStartBuffer = 0;
		tFrame.dwSpeed = 100;
		tFrame.dwTime = GetTickCount();
		BMPSCALE tScale;
		tScale.iWidth = 53;
		tScale.iHeight = 70;
		pEffect->SetPos(_pPlayer.m_vPos);
		pEffect->SetScale(Vec2(53.f, 35.f));
		pEffect->Initialize(tFrame, tScale, ((_pPlayer.m_vMoveDir.x > 0) ? L"acher_ult_ef_r" : L"acher_ult_ef_l"), RGB(0, 0, 0));
		CSceneManager::GetInstance()->GetCurScene()->AddObject(pEffect, OBJ_EFFECT);
	}

	if (_pPlayer.m_eCurState == CPlayer::ULT && (_pPlayer.m_tFrame.iFrameStart == _pPlayer.m_tFrame.iFrameEnd)
		|| _pPlayer.m_ePreState == CPlayer::ULT)
	{
		m_bIsTrigger = false;
		_pPlayer.m_bOnTarget = false;
		_pPlayer.m_pTarget = nullptr;
		_pPlayer.m_iTargetId = -1;
	}
		
}

void CUltAcher::Initialize(CPlayer& _pPlayer)
{
}

void CUltAcher::Use(CPlayer& _pPlayer, int _iDir)
{
	_pPlayer.Set_FrameKey(L"acher_ult_" + (_iDir > 0) ? L"r" : L"l");
	_pPlayer.SetState(CCharacter::ULT);
}

bool CUltAcher::IsFinished(CPlayer& _pPlayer, int _iDir)
{
	return false;
}
