#include "pch.h"
#include "CUltSwordman.h"
#include "CHitBox.h"
#include "CSceneManager.h"

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
	}
}

void CUltSwordman::Initialize(CPlayer& _pPlayer)
{
}

void CUltSwordman::Use(CPlayer& _pPlayer, int _iDir)
{
	_pPlayer.Set_FrameKey(L"swordman_ult_" + (_iDir > 0) ? L"r" : L"l");
	_pPlayer.SetState(CCharacter::ULT);

	Vec2 offset = Vec2(_iDir * 50.f, 0.f);
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
	pHitbox->SetScale(Vec2(60.f, 40.f));

	CSceneManager::GetInstance()->GetCurScene()->AddObject(pHitbox, OBJ_HITBOX);
}

bool CUltSwordman::IsFinished(CPlayer& _pPlayer, int _iDir)
{
	return false;
}
