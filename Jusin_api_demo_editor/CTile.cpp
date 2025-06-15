#include "pch.h"
#include "CTile.h"
#include "CBmpManager.h"
#include "CScrollManager.h"

CTile::CTile()
	: m_iDrawID(TILETYPE::BLUE_TURRET1), m_iOption(0)
{
}

CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	m_vScale.x = TILECX;
	m_vScale.y = TILECY;
}

int CTile::Update()
{
	__super::Update_Rect();

	return 0;
}

void CTile::Late_Update()
{
}

void CTile::Render(HDC _dc)
{
	HPEN hRedPen = HPEN();

	if (m_iOption == 0)
		return;
	else if (m_iOption == 1)
		hRedPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0)); // ← 두께 3
	else if (m_iOption == 2)
		hRedPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
	else
		hRedPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));

	HPEN hOldPen = (HPEN)SelectObject(_dc, hRedPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_dc, GetStockObject(NULL_BRUSH));

	float fScrollX = CScrollManager::Get_Instance()->Get_ScrollX() * g_fZoom;
	float fScrollY = CScrollManager::Get_Instance()->Get_ScrollY() * g_fZoom;

	float fDrawX = m_vPos.x * g_fZoom;
	float fDrawY = m_vPos.y * g_fZoom;
	float fHalfScaleX = (m_vScale.x * 0.5f) * g_fZoom;
	float fHalfScaleY = (m_vScale.y * 0.5f) * g_fZoom;

		//TODO: 타일
	Rectangle(_dc,
		fDrawX - fHalfScaleX + fScrollX,
		fDrawY - fHalfScaleY + fScrollY,
		fDrawX + fHalfScaleX + fScrollX,
		fDrawY + fHalfScaleY + fScrollY);

	MoveToEx(_dc,
		fDrawX - fHalfScaleX + fScrollX,
		fDrawY - fHalfScaleY + fScrollY,
		NULL);
	LineTo(_dc,
		fDrawX + fHalfScaleX + fScrollX,
		fDrawY + fHalfScaleY + fScrollY);

	MoveToEx(_dc,
		fDrawX + fHalfScaleX + fScrollX,
		fDrawY - fHalfScaleY + fScrollY,
		NULL);
	LineTo(_dc,
		fDrawX - fHalfScaleX + fScrollX,
		fDrawY + fHalfScaleY + fScrollY);

	SelectObject(_dc, hOldPen);
	SelectObject(_dc, hOldBrush);
	DeleteObject(hRedPen);

	WCHAR szCenter[64];
	swprintf_s(szCenter, 64, L"(%d,%d)", (int)m_iDrawID, (int)m_iOption);

	TextOut(_dc,
		(int)(fDrawX + fScrollX - 20),  // 가운데 정렬 보정
		(int)(fDrawY + fScrollY - 8),   // 수직 중앙 보정
		szCenter,
		lstrlen(szCenter));
}

void CTile::Release()
{
}
