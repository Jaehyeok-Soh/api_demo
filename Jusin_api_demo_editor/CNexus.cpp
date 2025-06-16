#include "pch.h"
#include "CNexus.h"
#include "CScrollManager.h"
#include "CBmpManager.h"

CNexus::CNexus()
	: strFrameBlueNexus(L"BlueNexus"), strFrameRedNexus(L"RedNexus")
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

	m_pFrameKey = L"Towers";

	m_iHP = 100;
}

int CNexus::Update()
{
	//TODO: �����ð����� �̴Ͼ� ��ȯ
	//TODO: Ǯ��?
	return NOEVENT;
}

void CNexus::Late_Update()
{
}

void CNexus::Render(HDC _dc)
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
		m_pFrameKey = strFrameBlueNexus;
		HDC hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);

		auto a = GdiTransparentBlt(_dc,
			drawX - spriteW / 2,
			drawY - spriteH / 2,
			spriteW,
			spriteH,
			hMemDC,
			0,
			0,
			157,   // ������ ��Ʈ�� ���� ���� ������
			168,
			RGB(189, 189, 189));   // ������ �ȼ� ���� ��
		auto b = 1;
	}
	else
	{
		m_pFrameKey = strFrameRedNexus;
		HDC hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);
		GdiTransparentBlt(_dc,
			drawX - spriteW / 2,
			drawY - spriteH / 2,
			spriteW,
			spriteH,
			hMemDC,
			0,
			0,
			161,   // ������ ��Ʈ�� ���� ���� ������
			208,
			RGB(189, 189, 189));   // ������ �ȼ� ���� ��
	}
}

void CNexus::Release()
{
}
