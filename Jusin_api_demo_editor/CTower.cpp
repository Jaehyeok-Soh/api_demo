#include "pch.h"
#include "CTower.h"
#include "CCollisionManager.h"

CTower::CTower()
{
}

CTower::~CTower()
{
}

void CTower::Initialize()
{
	//TODO: 체력 set
	//TODO: 공격력 set
	//TODO: 포지션 set
	//TODO: ID set
	//TODO: 넥서스, 억제기, 터렛 구분

	CreateCollider();

	GetCollider()->SetScale(Vec2(128.f, 128.f));

	m_tStatusInfo.m_iHp = 100;
	m_vScale = { 128.f, 128.f };
	m_fSpeed = 300.f;

	m_tAttackInfo.m_bIsAttack = false;
	m_tAttackInfo.m_fdtAttackTime = 0.f;
	m_tAttackInfo.m_fAttackDelay = 10.f;
	m_tAttackInfo.m_iDamage = 10;
}

void CTower::OnCollision(CCollider* _pOther)
{
	CCollisionManager::Collision_Rect_Resolve(GetCollider(), _pOther);
}

void CTower::OnCollisionEnter(CCollider* _pOther)
{
	CCollisionManager::Collision_Rect_Resolve(GetCollider(), _pOther);
}

