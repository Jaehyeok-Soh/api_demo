#include "pch.h"
#include "CWall.h"
#include "CCollisionManager.h"

CWall::CWall()
{
}

CWall::~CWall()
{
}

void CWall::Initialize()
{
	CreateCollider();
	auto collider = GetCollider();
	collider->SetScale(Vec2(4.f, 4.f));
	collider->Set_Layer(COL_WALL);
	collider->Set_Mask(COL_PLAYER | COL_MINION);
	
	__super::Update_Rect();
}

int CWall::Update()
{
	return NOEVENT;
}

void CWall::Late_Update()
{
	if (m_pCollider)
	{
		m_pCollider->Late_Update();
	}
}

void CWall::Render(HDC _dc)
{
	Component_Render(_dc);
}

void CWall::Release()
{
}

void CWall::OnCollisionEnter(CCollider* _pOther)
{
	CCollisionManager::Collision_Wall_Resolve(GetCollider(), _pOther);
}

void CWall::OnCollision(CCollider* _pOther)
{
	CCollisionManager::Collision_Wall_Resolve(GetCollider(), _pOther);
}