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
	//TODO: ü�� set
	//TODO: ���ݷ� set
	//TODO: ������ set
	//TODO: ID set
	//TODO: �ؼ���, ������, �ͷ� ����

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

