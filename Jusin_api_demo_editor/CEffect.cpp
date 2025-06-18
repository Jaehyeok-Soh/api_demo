#include "pch.h"
#include "CEffect.h"
#include "CScrollManager.h"
#include "CBmpManager.h"

CEffect::CEffect()
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CEffect::~CEffect()
{
}

void CEffect::Initialize()
{
}

void CEffect::Initialize(FRAME _fFrame, BMPSCALE _tScale, const TCHAR* _pFrameKey)
{
	m_tFrame = _fFrame;
	m_pFrameKey = _pFrameKey;
	m_tBmpScale = _tScale;
}

int CEffect::Update()
{
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		Set_Dead();

	if (m_bDead)
		return DEAD;

	__super::Update_Frame();

	return NOEVENT;
}

void CEffect::Late_Update()
{
}

void CEffect::Render(HDC _dc)
{
	int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	int drawX = int(m_vPos.x * g_fZoom + iScrollX * g_fZoom);
	int drawY = int(m_vPos.y * g_fZoom + iScrollY * g_fZoom);

	int spriteW = int(m_vScale.x * g_fZoom);
	int spriteH = int(m_vScale.y * g_fZoom);

	HDC   hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_dc,
		drawX - spriteW / 2,
		drawY - spriteH / 2,
		spriteW,
		spriteH,
		hMemDC,
		(int)m_tBmpScale.iWidth * m_tFrame.iFrameStart,
		(int)m_tBmpScale.iHeight * m_tFrame.iMotion,
		(int)m_tBmpScale.iWidth,   // 복사할 비트맵 가로 세로 사이즈
		(int)m_tBmpScale.iHeight,
		RGB(255, 255, 255));   // 제거할 픽셀 색상 값
}

void CEffect::Release()
{
}
