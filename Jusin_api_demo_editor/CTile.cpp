#include "pch.h"
#include "CTile.h"
#include "CBmpManager.h"

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
	//TODO: ≈∏¿œ
	Rectangle(_dc,
		m_vPos.x - (m_vScale.x * 0.5f),
		m_vPos.y - (m_vScale.y * 0.5f),
		m_vPos.x + (m_vScale.x * 0.5f),
		m_vPos.y + (m_vScale.y * 0.5f));

	MoveToEx(_dc, m_vPos.x - (m_vScale.x * 0.5f), m_vPos.y - (m_vScale.y * 0.5f), NULL);
	LineTo(_dc, m_vPos.x + (m_vScale.x * 0.5f), m_vPos.y + (m_vScale.y * 0.5f));

	MoveToEx(_dc, m_vPos.x + (m_vScale.x * 0.5f), m_vPos.y - (m_vScale.y * 0.5f), NULL);
	LineTo(_dc, m_vPos.x - (m_vScale.x * 0.5f), m_vPos.y + (m_vScale.y * 0.5f));
}

void CTile::Release()
{
}
