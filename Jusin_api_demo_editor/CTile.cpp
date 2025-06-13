#include "pch.h"
#include "CTile.h"
#include "CBmpManager.h"
#include "CScrollManager.h"

CTile::CTile()
	: m_iDrawID(0), m_iOption(0)
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
		hRedPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0)); // ← 두께 3
	else if (m_iOption == 1)
		hRedPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0)); // ← 두께 3
	else
		hRedPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0)); // ← 두께 3

	HPEN hOldPen = (HPEN)SelectObject(_dc, hRedPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_dc, GetStockObject(NULL_BRUSH));

	float fScrollX = CScrollManager::Get_Instance()->Get_ScrollX() * g_fZoom;
	float fScrollY = CScrollManager::Get_Instance()->Get_ScrollY() * g_fZoom;

	float fDrawX = m_vPos.x * g_fZoom;
	float fDrawY = m_vPos.y * g_fZoom;
	float fHalfScaleX = (m_vScale.x * 0.5f) * g_fZoom;
	float fHalfScaleY = (m_vScale.y * 0.5f) * g_fZoom;

	////TODO: 타일
	//Rectangle(_dc,
	//	m_vPos.x - (m_vScale.x * 0.5f) + fScrollX,
	//	m_vPos.y - (m_vScale.y * 0.5f) + fScrollY,
	//	m_vPos.x + (m_vScale.x * 0.5f) + fScrollX,
	//	m_vPos.y + (m_vScale.y * 0.5f) + fScrollY);

	//MoveToEx(_dc,
	//	m_vPos.x - (m_vScale.x * 0.5f) + fScrollX,
	//	m_vPos.y - (m_vScale.y * 0.5f) + fScrollY,
	//	NULL);
	//LineTo(_dc,
	//	m_vPos.x + (m_vScale.x * 0.5f) + fScrollX,
	//	m_vPos.y + (m_vScale.y * 0.5f) + fScrollY);

	//MoveToEx(_dc,
	//	m_vPos.x + (m_vScale.x * 0.5f) + fScrollX,
	//	m_vPos.y - (m_vScale.y * 0.5f) + fScrollY,
	//	NULL);
	//LineTo(_dc,
	//	m_vPos.x - (m_vScale.x * 0.5f) + fScrollX,
	//	m_vPos.y + (m_vScale.y * 0.5f) + fScrollY);

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

	//WCHAR buf[256];
	//swprintf_s(buf, 256, L"[Tile Pos] %.1f, %.1f | Scale %.1f, %.1f | Scroll %.1f, %.1f\n",
	//	m_vPos.x, m_vPos.y,
	//	m_vScale.x, m_vScale.y,
	//	fScrollX, fScrollY);
	//OutputDebugString(buf);
}

void CTile::Release()
{
}
