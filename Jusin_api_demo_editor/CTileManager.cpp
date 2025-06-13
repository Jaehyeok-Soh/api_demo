#include "pch.h"
#include "CTileManager.h"
#include "CObjectManager.h"
#include "CScrollManager.h"

CTileManager* CTileManager::m_pInstance = nullptr;

CTileManager::CTileManager()
{
	m_vecTile.reserve(TILEX * TILEY);
}

CTileManager::~CTileManager()
{
	Release();
}

void CTileManager::Initialize()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = (float)(TILECX * j) + (TILECX >> 1);
			float fY = (float)(TILECY * i) + (TILECY >> 1);

			CObject* pTile = CObjectManager::GetInstance()->CreateObject<CTile>(OBJ_TILE, fX, fY);
			CObjectManager::GetInstance()->AddObject(OBJ_TILE, pTile);
			m_vecTile.push_back(pTile);
		}
	}

}

void CTileManager::Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Update();
}

void CTileManager::Late_Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Late_Update();
}

void CTileManager::Render(HDC hDC)
{
	int	iCntX = abs((int)CScrollManager::Get_Instance()->Get_ScrollX() / TILECX);
	int	iCntY = abs((int)CScrollManager::Get_Instance()->Get_ScrollY() / TILECY);

	int iMaxX = iCntX + WINCX / TILECX + 2;
	int iMaxY = iCntY + WINCY / TILECY + 2;

	for (int i = iCntY; i < iMaxY; ++i)
	{
		for (int j = iCntX; j < iMaxX; ++j)
		{
			int		iIndex = i * TILEX + j;

			if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
				continue;

			m_vecTile[iIndex]->Render(hDC);
		}
	}
}

void CTileManager::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), DeleteObj());
	m_vecTile.clear();
}

void CTileManager::Picking_Tile(POINT ptMouse, int _iDrawID, int _iOption)
{
	int	x = ptMouse.x / TILECX;
	int y = ptMouse.y / TILECY;

	int		iIndex = y * TILEX + x;

	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->SetDrawID(_iDrawID);
	dynamic_cast<CTile*>(m_vecTile[iIndex])->SetOption(_iOption);
}
