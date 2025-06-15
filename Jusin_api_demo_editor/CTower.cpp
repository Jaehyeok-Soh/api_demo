#include "pch.h"
#include "CTower.h"

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

	GetCollider()->SetOffsetPos(Vec2(0.f, 65.f));
	GetCollider()->SetScale(Vec2(128.f, 128.f));

	m_iHP = 100;
	m_vScale = { 128.f, 128.f };
	m_fSpeed = 300.f;

	m_tAttackInfo.m_bIsAttack = false;
	m_tAttackInfo.m_fdtAttackTime = 0.f;
	m_tAttackInfo.m_fAttackDelay = 10.f;
	m_tAttackInfo.m_iDamage = 10;
}

