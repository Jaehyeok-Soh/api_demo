#include "pch.h"
#include "CInhibitor.h"

CInhibitor::CInhibitor()
{
}

CInhibitor::~CInhibitor()
{
}

void CInhibitor::Initialize()
{
	__super::Initialize();

	CreateCollider();

	//CreateGravity();
	GetCollider()->SetOffsetPos(Vec2(0.f, 65.f));
	GetCollider()->SetScale(Vec2(128.f, 128.f));

	m_iHP = 100;
}

int CInhibitor::Update()
{
	__super::Update();
	return NOEVENT;
}

void CInhibitor::Late_Update()
{
}

void CInhibitor::Render(HDC _dc)
{
	__super::Render(_dc);
}

void CInhibitor::Release()
{
}
