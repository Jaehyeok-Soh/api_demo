#include "pch.h"
#include "CRanged.h"
#include "CBullet.h"
#include "CSceneManager.h"
#include "CTimeManager.h"
#include "CCharacter.h"

CRanged::CRanged()
{
}

CRanged::~CRanged()
{
}

int CRanged::Update()
{
	__super::Update();
	return NOEVENT;
}

void CRanged::Late_Update()
{
	__super::Late_Update();
}

void CRanged::Render(HDC _dc)
{
}

void CRanged::Release()
{
}

void CRanged::Attack()
{
	CObject* pBullet = new CBullet();
	static_cast<CBullet*>(pBullet)->Initialize(m_pOwner->GetTarget());
	pBullet->SetPos(m_pOwner->GetPos());
	pBullet->SetScale(Vec2(8.f, 8.f));
	static_cast<CBullet*>(pBullet)->SetBulletWeapon(this);
	CSceneManager::GetInstance()->GetCurScene()->AddObject(pBullet, OBJ_ATTACK);

	m_tAttackInfo.m_fdtAttackTime += fDT;
	m_tAttackInfo.m_bIsAttack = true;
}

void CRanged::OnHit()
{
	if (m_pOwner->GetTarget() == nullptr)
		return;

	if (static_cast<CCharacter*>(m_pOwner->GetTarget()))
		static_cast<CCharacter*>(m_pOwner->GetTarget())->OnHit(m_tAttackInfo.m_iDamage);
}
