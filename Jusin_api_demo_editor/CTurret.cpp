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
	__super::Update();
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
			drawY - spriteH / 2 - 200,
			spriteW,
			spriteH + 200,
			hMemDC,
			0,
			0,
			162,   // ������ ��Ʈ�� ���� ���� ������
			168,
			RGB(189, 189, 189));   // ������ �ȼ� ���� ��
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
			144,   // ������ ��Ʈ�� ���� ���� ������
			190,
			RGB(189, 189, 189));   // ������ �ȼ� ���� ��
	}
}

void CTurret::Release()
{
}

void CTurret::CreateWeapon()
{
}
