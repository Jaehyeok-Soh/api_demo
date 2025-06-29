#include "pch.h"
#include "CPngEffect.h"
#include "CScrollManager.h"
#include "CBmpManager.h"
#include "CBlendingManager.h"

CPngEffect::CPngEffect()
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CPngEffect::~CPngEffect()
{
}

void CPngEffect::Initialize()
{
}

void CPngEffect::Initialize(FRAME _fFrame, BMPSCALE _tScale, const TCHAR* _pFrameKey, COLORREF _rgb)
{
	m_tFrame = _fFrame;
	m_pFrameKey = _pFrameKey;
	m_tBmpScale = _tScale;
	m_rgbColor = _rgb;
}

int CPngEffect::Update()
{
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		Set_Dead();

	if (m_bDead)
		return DEAD;

	__super::Update_Frame();

	return NOEVENT;
}

void CPngEffect::Late_Update()
{
}

void CPngEffect::Render(HDC _dc)
{
	int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	int drawX = int(m_vPos.x * g_fZoom + iScrollX * g_fZoom);
	int drawY = int(m_vPos.y * g_fZoom + iScrollY * g_fZoom);

	int spriteW = int(m_vScale.x * g_fZoom);
	int spriteH = int(m_vScale.y * g_fZoom);

	CBlendingManager::GetInstance()->Render(_dc,
		L"../Image/ApiDemo/Character/spawn/spwan_ef_1row.png",
		Rect(drawX - spriteW / 2, drawY - spriteW / 2, spriteW, spriteW),
		(int)m_vScale.x * m_tFrame.iFrameStart,
		(int)m_vScale.y * m_tFrame.iMotion,
		(int)m_vScale.x,
		(int)m_vScale.y,
		1.f);
}

void CPngEffect::Release()
{
}
