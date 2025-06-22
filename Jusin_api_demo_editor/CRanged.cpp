#include "pch.h"
#include "CRanged.h"
#include "CBullet.h"
#include "CSceneManager.h"
#include "CTimeManager.h"

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
	pBullet->SetScale(Vec2(32.f, 32.f));
	CSceneManager::GetInstance()->GetCurScene()->AddObject(pBullet, OBJ_ATTACK);

	m_tAttackInfo.m_fdtAttackTime += fDT;
	m_tAttackInfo.m_bIsAttack = true;
}
