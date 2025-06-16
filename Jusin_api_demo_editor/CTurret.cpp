#include "pch.h"
#include "CTurret.h"
#include "CScrollManager.h"
#include "CBmpManager.h"

CTurret::CTurret()
	:strFrameBlueTurret(L"BlueTurret"), strFrameRedTurret(L"RedTurret")
{
}

CTurret::~CTurret()
{
}

void CTurret::Initialize()
{
	__super::Initialize();

	CreateCollider();

	//CreateGravity();
	GetCollider()->SetOffsetPos(Vec2(0.f, 65.f));
	GetCollider()->SetScale(Vec2(32.f, 32.f));

	m_iHP = 100;
}

int CTurret::Update()
{
	__super::Update();
	return NOEVENT;
}

void CTurret::Late_Update()
{
}

void CTurret::Render(HDC _dc)
{
	Component_Render(_dc);
	int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	int drawX = int(m_vPos.x * g_fZoom + iScrollX * g_fZoom);
	int drawY = int(m_vPos.y * g_fZoom + iScrollY * g_fZoom);

	int spriteW = int(m_vScale.x * g_fZoom);
	int spriteH = int(m_vScale.y * g_fZoom);

	if (m_bTeam)
	{
		m_pFrameKey = strFrameBlueTurret;
		HDC hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);

		auto a = GdiTransparentBlt(_dc,
			drawX - spriteW / 2,
			drawY - spriteH / 2 - 200,
			spriteW,
			spriteH + 200,
			hMemDC,
			0,
			0,
			162,   // 복사할 비트맵 가로 세로 사이즈
			168,
			RGB(189, 189, 189));   // 제거할 픽셀 색상 값
		auto b = 1;
	}
	else
	{
		m_pFrameKey = strFrameRedTurret;
		HDC hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);
		GdiTransparentBlt(_dc,
			drawX - spriteW / 2,
			drawY - spriteH / 2 - 200,
			spriteW,
			spriteH + 200,
			hMemDC,
			0,
			0,
			144,   // 복사할 비트맵 가로 세로 사이즈
			190,
			RGB(189, 189, 189));   // 제거할 픽셀 색상 값
	}
}

void CTurret::Release()
{
}
