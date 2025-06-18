#include "pch.h"
#include "CWeapon.h"
#include "CTimeManager.h"

CWeapon::CWeapon()
	: m_pOwner(nullptr)
{
	ZeroMemory(&m_tAttackInfo, sizeof(ATTACKINFO));
}

CWeapon::~CWeapon()
{
}

void CWeapon::Initialize()
{
	CreateCollider();

	Vec2 vOwnerPos = m_pOwner->GetPos();
	Vec2 vOwnerDir = m_pOwner->GetDir();

	GetCollider()->SetOffsetPos(Vec2(0.f, 65.f));
	GetCollider()->SetScale(Vec2(8.f, 8.f));
	GetCollider()->Set_Layer(COL_SKILL);
	GetCollider()->Set_Mask(COL_MINION
		| COL_TOWER
		| COL_ATTACK
		| COL_PLAYER);


	m_fDistance = 50.f;
	m_vPos = Vec2{ vOwnerPos.x + (m_fDistance * vOwnerDir.x), vOwnerPos.y };
}

void CWeapon::Initialize(CObject* _pOwner, ATTACKINFO _tAttackInfo)
{
	m_pOwner = _pOwner;
	m_tAttackInfo = _tAttackInfo;
	Initialize();
}

int CWeapon::Update()
{
	if (m_tAttackInfo.m_bIsAttack)
	{
		AttackTime_Update();
		OwnerPos_Update();
	}

	__super::Update_Rect();

	return NOEVENT;
}

void CWeapon::Late_Update()
{
	if (m_pCollider)
		m_pCollider->Late_Update();
}

void CWeapon::Render(HDC _dc)
{
}

void CWeapon::Release()
{
}

void CWeapon::Attack()
{
	m_tAttackInfo.m_fdtAttackTime += fDT;

	GetCollider()->SetActivate(true);
	m_tAttackInfo.m_bIsAttack = true;
}

void CWeapon::AttackTime_Update()
{
	m_tAttackInfo.m_fdtAttackTime += fDT;
	if (m_tAttackInfo.m_fdtAttackTime > m_tAttackInfo.m_fAttackDelay)
	{
		m_tAttackInfo.m_fdtAttackTime = 0.f;
		GetCollider()->SetActivate(false);
		m_tAttackInfo.m_bIsAttack = false;
	}
}

void CWeapon::OwnerPos_Update()
{
	Vec2 vOwnerPos = m_pOwner->GetPos();
	Vec2 vOwnerDir = m_pOwner->GetDir();
	m_vPos = Vec2{ vOwnerPos.x + (m_fDistance * vOwnerDir.x), vOwnerPos.y };
}
