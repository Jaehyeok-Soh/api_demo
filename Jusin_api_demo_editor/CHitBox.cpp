#include "pch.h"
#include "CHitBox.h"
#include "CTimeManager.h"
#include "CCharacter.h"

CHitbox::CHitbox()
{
	ZeroMemory(&m_tHitboxInfo, sizeof(m_tHitboxInfo));
}

CHitbox::~CHitbox()
{
	Release();
}

void CHitbox::Initialize()
{

}

void CHitbox::Initialize(HITBOXINFO _tVal)
{
	m_tHitboxInfo = _tVal;
	CreateCollider();

	GetCollider()->SetScale(m_vScale);
	GetCollider()->Set_Layer(COL_PLAYER);
	GetCollider()->Set_Mask(COL_MINION
		| COL_PLAYER);
}

int CHitbox::Update()
{
	m_tHitboxInfo.m_fElapsed += fDT;
	if (m_tHitboxInfo.m_fElapsed >= m_tHitboxInfo.m_fDuration)
		Set_Dead();

	if (m_bDead)
		return DEAD;

	__super::Update_Rect();

	return NOEVENT;
}

void CHitbox::Late_Update()
{
	if (m_pCollider)
		m_pCollider->Late_Update();
}

void CHitbox::Render(HDC _dc)
{
}

void CHitbox::Release()
{
}

void CHitbox::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->GetOwner()->GetTeam() == m_tHitboxInfo.m_pOwner->GetTeam())
		return;

	if (m_tHitboxInfo.m_bHitApplied)
		return;

	if ((GetCollider()->Get_Layer() & _pOther->Get_Mask()) != 0)
	{
		static_cast<CCharacter*>(_pOther->GetOwner())->OnHit(m_tHitboxInfo.m_iDamage);
		if (m_tHitboxInfo.m_bOnce)
			m_tHitboxInfo.m_bHitApplied = true;
	} 
}
