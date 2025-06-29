#include "pch.h"
#include "CSkillAcher.h"
#include "CHitBox.h"
#include "CEffect.h"
#include "CSceneManager.h"
#include "CBullet.h"
#include "CTileManager.h"

CSkillAcher::CSkillAcher()
{
}

CSkillAcher::~CSkillAcher()
{
}

void CSkillAcher::Update(CPlayer& _pPlayer)
{
	if (_pPlayer.m_eCurState == CPlayer::SKILL)
	{
		Vec2 offset = Vec2(_pPlayer.m_vMoveDir.x * 5.f, 0.f);
		Vec2 pos = _pPlayer.GetPos() + offset;

		_pPlayer.FindTarget();

		CObject* pBullet = new CBullet();
		static_cast<CBullet*>(pBullet)->Initialize(_pPlayer.GetTarget());
		pBullet->SetPos(_pPlayer.GetPos());
		pBullet->SetScale(Vec2(4.f, 4.f));
		static_cast<CBullet*>(pBullet)->SetBulletWeapon(_pPlayer.m_pWeapon);
		CSceneManager::GetInstance()->GetCurScene()->AddObject(pBullet, OBJ_ATTACK);

		if (!m_bIsTrigger)
		{
			m_bIsTrigger = true;

			auto tileManager = CTileManager::Get_Instance();
			float distX = 16.f;
			float distY = 8.f;
			while (tileManager->CheckPeekDisable((_pPlayer.m_vPos.x + distX * ((_pPlayer.m_vMoveDir.x > 0) ? -1.f : 1.f)) / TILECX, (_pPlayer.m_vPos.y - distY) / TILECY))
			{
				distX -= 4.f;
				distY -= 4.f;

				if (distX == 0.f || distY == 0.f)
					distX = 0.f; distY = 0.f; return;
			}

			_pPlayer.m_vPos.x = _pPlayer.m_vPos.x + distX * ((_pPlayer.m_vMoveDir.x > 0) ? -1.f : 1.f);
			_pPlayer.m_vPos.y = _pPlayer.m_vPos.y - distY;

			CEffect* pEffect = new CEffect();
			FRAME tFrame;
			tFrame.iFrameStart = 0;
			tFrame.iFrameEnd = 3;
			tFrame.iMotion = 0;
			tFrame.iStartBuffer = 0;
			tFrame.dwSpeed = 200;
			tFrame.dwTime = GetTickCount();
			BMPSCALE tScale;
			tScale.iWidth = 53;
			tScale.iHeight = 70;
			pEffect->SetPos(pos);
			pEffect->SetScale(Vec2(38.f, 27.f));
			pEffect->Initialize(tFrame, tScale, ((_pPlayer.m_vMoveDir.x > 0) ? L"acher_skill_ef_r" : L"acher_skill_ef_l"), RGB(0, 0, 0));
			CSceneManager::GetInstance()->GetCurScene()->AddObject(pEffect, OBJ_EFFECT);
		}
	}

	if (_pPlayer.m_eCurState == CPlayer::SKILL && (_pPlayer.m_tFrame.iFrameStart == _pPlayer.m_tFrame.iFrameEnd)
		|| _pPlayer.m_ePreState == CPlayer::SKILL)
	{
		m_bIsTrigger = false;
		_pPlayer.m_bOnTarget = false;
		_pPlayer.m_pTarget = nullptr;
		_pPlayer.m_iTargetId = -1;
	}
}

void CSkillAcher::Initialize(CPlayer& _pPlayer)
{
}

void CSkillAcher::Use(CPlayer& _pPlayer, int _iDir)
{
	_pPlayer.Set_FrameKey(L"acher_skill_" + (_iDir > 0) ? L"r" : L"l");
	_pPlayer.SetState(CCharacter::SKILL);
}

bool CSkillAcher::IsFinished(CPlayer& _pPlayer, int _iDir)
{
	return false;
}
