#include "pch.h"
#include "CNexus.h"

CNexus::CNexus()
{
}

CNexus::~CNexus()
{
}

void CNexus::Initialize()
{
	__super::Initialize();

	CreateCollider();

	//CreateGravity();
	GetCollider()->SetOffsetPos(Vec2(0.f, 65.f));
	GetCollider()->SetScale(Vec2(64.f, 64.f));

	m_iHP = 100;
}

int CNexus::Update()
{
	return NOEVENT;
}

void CNexus::Late_Update()
{
}

void CNexus::Render(HDC _dc)
{
	Component_Render(_dc);
}

void CNexus::Release()
{
}
