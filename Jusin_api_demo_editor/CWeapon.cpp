#include "pch.h"
#include "CWeapon.h"

CWeapon::CWeapon()
	: pOwner(nullptr)
{
}

CWeapon::~CWeapon()
{
	Release();
}

void CWeapon::Initialize()
{
	CreateCollider();

	GetCollider()->SetOffsetPos(Vec2(0.f, 65.f));
	GetCollider()->SetScale(Vec2(8.f, 8.f));
	GetCollider()->Set_Layer(COL_SKILL);
	GetCollider()->Set_Mask(COL_MINION
		| COL_TOWER
		| COL_ATTACK
		| COL_PLAYER);

	GetCollider()->SetScale(Vec2(8.f, 8.f));

}

int CWeapon::Update()
{
	return 0;
}

void CWeapon::Late_Update()
{
}

void CWeapon::Render(HDC _dc)
{
}

void CWeapon::Release()
{
}
