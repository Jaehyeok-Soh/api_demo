#include "pch.h"
#include "CInhibitor.h"
#include "CScrollManager.h"
#include "CBmpManager.h"

CInhibitor::CInhibitor()
	:strFrameBlueInhibitor(L"BlueInhibitor"), strFrameRedInhibitor(L"RedInhibitor")
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
	GetCollider()->SetScale(Vec2(32.f, 32.f));
	GetCollider()->Set_Layer(COL_TOWER);
	GetCollider()->Set_Mask(COL_MINION
		| COL_TOWER
		| COL_ATTACK
		| COL_PLAYER
		| COL_SKILL);

	m_iHP = 100;
}

int CInhibitor::Update()
{
	__super::Update();
	return NOEVENT;
}

void CInhibitor::Late_Update()
{
	if (m_pCollider)
		m_pCollider->Late_Update();

	/*if (m_pGravity)
		m_pGravity->Late_Update();*/
}

void CInhibitor::Render(HDC _dc)
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
		m_pFrameKey = strFrameBlueInhibitor;
		HDC hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);

		auto a = GdiTransparentBlt(_dc,
			drawX - spriteW / 2,
			drawY - spriteH / 2 - 200,
			spriteW,
			spriteH + 200,
			hMemDC,
			0,
			0,
			106,   // 복사할 비트맵 가로 세로 사이즈
			121,
			RGB(189, 189, 189));   // 제거할 픽셀 색상 값
		auto b = 1;
	}
	else
	{
		m_pFrameKey = strFrameRedInhibitor;
		HDC hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);
		GdiTransparentBlt(_dc,
			drawX - spriteW / 2,
			drawY - spriteH / 2 - 200,
			spriteW,
			spriteH + 200,
			hMemDC,
			0,
			0,
			108,   // 복사할 비트맵 가로 세로 사이즈
			125,
			RGB(189, 189, 189));   // 제거할 픽셀 색상 값
	}
}

void CInhibitor::Release()
{
}
