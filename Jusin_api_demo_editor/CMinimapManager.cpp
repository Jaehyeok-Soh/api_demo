#include "pch.h"
#include "CMinimapManager.h"
#include "CKeyManager.h"
#include "CScrollManager.h"
#include "CTileManager.h"
#include "CAStarManager.h"

CMinimapManager* CMinimapManager::m_pInstance = nullptr;

CMinimapManager::CMinimapManager()
{
	ZeroMemory(&m_vMapRatio, sizeof(Vec2));
	ZeroMemory(&m_vPos, sizeof(Vec2));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tMiniMapInfo, sizeof(INFO));
	ZeroMemory(&m_tMapInfo, sizeof(INFO));
}

CMinimapManager::~CMinimapManager()
{
}

void CMinimapManager::Initialize(CObject* _pObj)
{
	//0
	//0
	//1280
	//720
	m_tMapInfo = { 0, 0, 720, 405 };
	
	//WINCX - 200
	//WINCY - 200
	//200
	//200
	m_tMiniMapInfo = { WINCX - 356.f, WINCY - 200.f, 354.f, 200.f };

	m_vScale = Vec2(125.f, 75.f);

	m_vMapRatio = Vec2(m_tMapInfo.fCX / m_tMiniMapInfo.fCX, m_tMapInfo.fCY / m_tMiniMapInfo.fCY);

	m_pPlayer = static_cast<CPlayer*>(_pObj);
}

void CMinimapManager::Update()
{
	UpdateScroll();

	KeyInput();
	UpdateRect();
	UpdateEllipse();

	RectPadding();
	EllipsePadding();

	CalcPlayerPos();
}

void CMinimapManager::Render(HDC _dc)
{
	DrawPath(_dc);
	DrawPlayer(_dc);
	DrawRect(_dc);
}

void CMinimapManager::RectPadding()
{
	if (m_tRect.left < m_tMiniMapInfo.fX)
	{
		m_tRect.left += 1.f;
	}

	if (m_tRect.top < m_tMiniMapInfo.fY)
	{
		m_tRect.top += 1.f;
	}

	if (m_tRect.right > m_tMiniMapInfo.fCX)
	{
		m_tRect.top -= 1.f;
	}

	if (m_tRect.bottom > m_tMiniMapInfo.fCY)
	{
		m_tRect.bottom -= 1.f;
	}
}

void CMinimapManager::DrawPath(HDC _dc)
{
	if (m_pPlayer->m_Path.empty())
		return;

	HPEN hPen = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
	HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	HPEN hOldPen = (HPEN)SelectObject(_dc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_dc, hBrush);

	Vec2 first = m_pPlayer->m_Path.front();

	float ptX = first.x / m_tMapInfo.fCX * m_tMiniMapInfo.fCX + m_tMiniMapInfo.fX;
	float ptY = first.y / m_tMapInfo.fCY * m_tMiniMapInfo.fCY + m_tMiniMapInfo.fY;

	MoveToEx(_dc, ptX, ptY, nullptr);

	for (auto path : m_pPlayer->m_Path)
	{
		if (path == first)
			continue;
		ptX = path.x / m_tMapInfo.fCX * m_tMiniMapInfo.fCX + m_tMiniMapInfo.fX;
		ptY = path.y / m_tMapInfo.fCY * m_tMiniMapInfo.fCY + m_tMiniMapInfo.fY;
		LineTo(_dc,
			ptX,
			ptY);
	}

	SelectObject(_dc, hOldPen);
	SelectObject(_dc, hOldBrush);
	DeleteObject(hPen);
}

void CMinimapManager::EllipsePadding()
{
	if (m_tPlayerRect.left < m_tMiniMapInfo.fX)
	{
		m_tPlayerRect.left += 1.f;
	}

	if (m_tPlayerRect.top < m_tMiniMapInfo.fY)
	{
		m_tRect.top += 1.f;
	}

	if (m_tPlayerRect.right > m_tMiniMapInfo.fCX)
	{
		m_tRect.top -= 1.f;
	}

	if (m_tPlayerRect.bottom > m_tMiniMapInfo.fCY)
	{
		m_tRect.bottom -= 1.f;
	}
}

void CMinimapManager::UpdateEllipse()
{
	m_tPlayerRect = { (long)(m_vPlayerPos.x - 5.f),
				(long)(m_vPlayerPos.y - 5.f),
				(long)(m_vPlayerPos.x + 5.f),
				(long)(m_vPlayerPos.y + 5.f) };
}

void CMinimapManager::CalcPlayerPos()
{
	float fWidthRatio = m_pPlayer->m_vPos.x / m_tMapInfo.fCX;
	float fHeightRatio = m_pPlayer->m_vPos.y / m_tMapInfo.fCY;

	float fMiniMapWidthRatio = (fWidthRatio * m_tMiniMapInfo.fCX);
	float fMiniMapHeightRatio = (fHeightRatio * m_tMiniMapInfo.fCY);

	m_vPlayerPos = Vec2(m_tMiniMapInfo.fX + fMiniMapWidthRatio, m_tMiniMapInfo.fY + fMiniMapHeightRatio);
}

void CMinimapManager::DrawPlayer(HDC _dc)
{
	Ellipse(_dc,
		(int)(m_tPlayerRect.left),
		(int)(m_tPlayerRect.top),
		(int)(m_tPlayerRect.right),
		(int)(m_tPlayerRect.bottom));
}

void CMinimapManager::UpdateScroll()
{
	auto scrollManager = CScrollManager::Get_Instance();
	float fScrollX = abs(scrollManager->Get_ScrollX());
	float fScrollY = abs(scrollManager->Get_ScrollY());

	//1280
	//720
	float fWidthRatio = fScrollX / m_tMapInfo.fCX;
	float fHeightRatio = fScrollY / m_tMapInfo.fCY;

	float a = ((fWidthRatio * m_tMiniMapInfo.fCX) + m_tMiniMapInfo.fX) - 50.f;
	float b = ((fHeightRatio * m_tMiniMapInfo.fCY) + m_tMiniMapInfo.fY) - 25.f;

	m_vPos = Vec2(a + m_vScale.x, b + m_vScale.y);
	//m_vPos = Vec2(a, b);
}

void CMinimapManager::DrawRect(HDC _dc)
{
	HPEN hPen = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
	HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	HPEN hOldPen = (HPEN)SelectObject(_dc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_dc, hBrush);

	Rectangle(_dc,
		(int)m_tRect.left,
		(int)m_tRect.top,
		(int)m_tRect.right,
		(int)m_tRect.bottom);

	SelectObject(_dc, hOldPen);
	SelectObject(_dc, hOldBrush);
	DeleteObject(hPen);
}

void CMinimapManager::UpdateRect()
{
	m_tRect = { (long)(m_vPos.x - m_vScale.x / 2.f),
				(long)(m_vPos.y - m_vScale.y / 2.f),
				(long)(m_vPos.x + m_vScale.x / 2.f),
				(long)(m_vPos.y + m_vScale.y / 2.f) };
}

void CMinimapManager::KeyInput()
{
	auto keyManager = CKeyManager::Get_Instance();

	if (keyManager->Key_Pressing(VK_LBUTTON))
	{
		if (g_ptMousePos.x > m_tMiniMapInfo.fX 
			&& g_ptMousePos.y > m_tMiniMapInfo.fY
			&& g_ptMousePos.x < WINCX
			&& g_ptMousePos.y < WINCY)
		{
			Vec2 oldPos = m_vPos;

			auto scrollManager = CScrollManager::Get_Instance();
			m_vPos = Vec2((float)g_ptMousePos.x, (float)g_ptMousePos.y);

			float diffX = oldPos.x - m_vPos.x;
			float diffY = oldPos.y - m_vPos.y;

			scrollManager->Set_ScrollX(diffX * 1.77f);
			scrollManager->Set_ScrollY(diffY * 1.77f);
		}
	}
	
	if (keyManager->Key_Pressing(VK_RBUTTON))
	{
		if (g_ptMousePos.x > m_tMiniMapInfo.fX
			&& g_ptMousePos.y > m_tMiniMapInfo.fY
			&& g_ptMousePos.x < WINCX
			&& g_ptMousePos.y < WINCY)
		{
			float ptMinimapX = (g_ptMousePos.x - m_tMiniMapInfo.fX) / m_tMiniMapInfo.fCX;
			float ptMinimapY = (g_ptMousePos.y - m_tMiniMapInfo.fY) / m_tMiniMapInfo.fCY;

			float worldX = m_tMapInfo.fCX * ptMinimapX;
			float worldY = m_tMapInfo.fCY * ptMinimapY;

			POINT pt = { worldX, worldY };
			if (CTileManager::Get_Instance()->Peeking_Tile(pt))
			{
				Vec2 vTilePos = CTileManager::Get_Instance()->MouseToTile(pt);
				vTilePos = Vec2((int)vTilePos.x / TILECX, (int)vTilePos.y / TILECY);
				Vec2 startIdx = Vec2((int)(m_pPlayer->m_vPos.x / TILECX), (int)(m_pPlayer->m_vPos.y / TILECY));
				Vec2 endIdx = vTilePos;

				auto future = async(launch::async,
					&CAStarManager::FindPath,
					CAStarManager::GetInstance(),
					startIdx, endIdx);

				m_pPlayer->m_Path = future.get();

				if (!m_pPlayer->m_Path.empty())
				{
					m_pPlayer->m_Path.pop_front();
					if (m_pPlayer->m_eCurState == ATTACK)
						m_pPlayer->m_tFrame.iFrameStart = 0;
					m_pPlayer->AttackInit();
					m_pPlayer->m_eCurState = CPlayer::RUN;
					m_pPlayer->m_bOnTarget = false;
					m_pPlayer->m_pTarget = nullptr;
				}
			}

		}
	}
}
