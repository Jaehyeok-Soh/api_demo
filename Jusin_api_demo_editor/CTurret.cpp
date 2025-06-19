#include "pch.h"
#include "CTurret.h"
#include "CScrollManager.h"
#include "CBmpManager.h"
#include "CKeyManager.h"
#include "CPeekingManager.h"

CTurret::CTurret()
	:strFrameBlueTurret(L"BlueTurretSmall"), strFrameRedTurret(L"RedTurretSmall")
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
	GetCollider()->Set_Layer(COL_TOWER);
	GetCollider()->Set_Mask(COL_MINION
		| COL_TOWER
		| COL_ATTACK
		| COL_PLAYER
		| COL_SKILL);

	m_tStatusInfo.m_iHp = 100;
}

int CTurret::Update()
{
	POINT ptMouse;
	GetCursorPos(&ptMouse); // 화면 좌표
	ScreenToClient(g_hWnd, &ptMouse); // 클라이언트 좌표로 변환

	POINT vWorldMouse;
	vWorldMouse.x = ptMouse.x / g_fZoom - CScrollManager::Get_Instance()->Get_ScrollX();
	vWorldMouse.y = ptMouse.y / g_fZoom - CScrollManager::Get_Instance()->Get_ScrollY();

	if (PtInRect(&m_tRect, vWorldMouse))
	{
		if (CKeyManager::Get_Instance()->Key_Pressing(VK_RBUTTON))
		{
			CPeekingManager::GetInstance()->OnPeek(this);
		}
	}

	//__super::Update();
	__super::Update_Rect();
	return NOEVENT;
}

void CTurret::Late_Update()
{
	if (m_pCollider)
		m_pCollider->Late_Update();

	/*if (m_pGravity)
		m_pGravity->Late_Update();*/
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
			drawY - spriteH / 2 - 50,
			spriteW,
			spriteH + 50,
			hMemDC,
			0,
			0,
			87,   // 복사할 비트맵 가로 세로 사이즈
			149,
			RGB(189, 189, 189));   // 제거할 픽셀 색상 값
		auto b = 1;
	}
	else
	{
		m_pFrameKey = strFrameRedTurret;
		HDC hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);
		GdiTransparentBlt(_dc,
			drawX - spriteW / 2,
			drawY - spriteH / 2 - 50,
			spriteW,
			spriteH + 50,
			hMemDC,
			0,
			0,
			94,   // 복사할 비트맵 가로 세로 사이즈
			169,
			RGB(189, 189, 189));   // 제거할 픽셀 색상 값
	}

#pragma region 테스트용
	std::wstring wstrHP = std::to_wstring(m_tStatusInfo.m_iHp);
	LPCWSTR szHP = wstrHP.c_str();
	TextOut(_dc,
		(int)drawX + 60,
		(int)drawY - 70,
		szHP,
		lstrlen(szHP));
#pragma endregion
}

void CTurret::Release()
{
}

void CTurret::CreateWeapon()
{
}
