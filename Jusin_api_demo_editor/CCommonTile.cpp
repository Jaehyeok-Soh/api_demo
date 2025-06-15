#include "pch.h"
#include "CCommonTile.h"

CCommonTile::CCommonTile()
{
}

CCommonTile::~CCommonTile()
{
	Release();
}

void CCommonTile::Initialize()
{
	__super::Initialize();
}

int CCommonTile::Update()
{
	__super::Update();
	return NOEVENT;
}

void CCommonTile::Late_Update()
{
}

void CCommonTile::Render(HDC _dc)
{
	__super::Render(_dc);
}

void CCommonTile::Release()
{
}
