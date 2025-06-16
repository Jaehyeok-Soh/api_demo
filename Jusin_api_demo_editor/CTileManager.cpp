#include "pch.h"
#include "CTileManager.h"
#include "CObjectManager.h"
#include "CScrollManager.h"
#include "CSpawnNexus.h"
#include "CSpawnTurret.h"
#include "CSpawnInhibitor.h"
#include "CCommonTile.h"

CTileManager* CTileManager::m_pInstance = nullptr;

CTileManager::CTileManager()
	: iCntX(0), iCntY(0), iMaxX(0), iMaxY(0)
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

			CObject* pTile = CObjectManager::GetInstance()->CreateObject<CCommonTile>(OBJ_TILE, fX, fY);
			pTile->Initialize();
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
	iCntX = abs((int)CScrollManager::Get_Instance()->Get_ScrollX() / TILECX);
	iCntY = abs((int)CScrollManager::Get_Instance()->Get_ScrollY() / TILECY);

	iMaxX = iCntX + WINCX / TILECX + 2;
	iMaxY = iCntY + WINCY / TILECY + 2;

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

TILETYPE CTileManager::Peeking_Tile(POINT ptMouse)
{
	int	x = ptMouse.x / TILECX;
	int y = ptMouse.y / TILECY;

	int		iIndex = y * TILEX + x;

	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
		return TILETYPE::PEEK_DISABLE;

	int iOption = static_cast<CTile*>(m_vecTile[iIndex])->GetOption();
	if (iOption == 0)
		return TILETYPE::PEEK_DISABLE;

	return (TILETYPE)iOption;
}

void CTileManager::Drawing_Tile(POINT ptMouse, TILETYPE _iDrawID, int _iOption)
{
	int	x = ptMouse.x / TILECX;
	int y = ptMouse.y / TILECY;

	int		iIndex = y * TILEX + x;

	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->SetDrawID(_iDrawID);
	dynamic_cast<CTile*>(m_vecTile[iIndex])->SetOption(_iOption);
}

void CTileManager::Save_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile.dat",
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	int iDrawID(0), iOption(0);
	DWORD dwByte(0);

	for (auto& pTile : m_vecTile)
	{
		iDrawID = dynamic_cast<CTile*>(pTile)->GetDrawID();
		iOption = dynamic_cast<CTile*>(pTile)->GetOption();

		WriteFile(hFile, &(pTile->GetPos()), sizeof(Vec2), &dwByte, NULL);
		WriteFile(hFile, &(pTile->GetScale()), sizeof(Vec2), &dwByte, NULL);
		WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, NULL);
		WriteFile(hFile, &iOption, sizeof(int), &dwByte, NULL);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Tile Save", L"Complete", MB_OK);
}

void CTileManager::Load_Tile()
{
	HANDLE	hFile = CreateFile(L"../Data/Tile.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	Release();

	TILETYPE	iDrawID(TILETYPE::BLUE_TURRET1);
	int iOption(0);
	DWORD	dwByte(0);
	Vec2	vPos{};
	Vec2	vScale{};

	while (true)
	{
		ReadFile(hFile, &vPos, sizeof(Vec2), &dwByte, NULL);
		ReadFile(hFile, &vScale, sizeof(Vec2), &dwByte, NULL);
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, NULL);
		ReadFile(hFile, &iOption, sizeof(int), &dwByte, NULL);

		if (0 == dwByte)
			break;

		CObject* pTile = nullptr;

		if (iDrawID == BLUE_NEXUS || iDrawID == RED_NEXUS)
		{
			pTile = new CSpawnNexus();
		}
		else if (iDrawID == BLUE_INHIBITER || iDrawID == RED_INHIBITER)
		{
			pTile = new CSpawnInhibitor();
		}
		else if (iDrawID == BLUE_TURRET1 
			|| iDrawID == BLUE_TURRET2
			|| iDrawID == RED_TURRET1
			|| iDrawID == RED_TURRET2)
		{
			pTile = new CSpawnTurret();
		}
		else
		{
			pTile = new CCommonTile();
		}

		dynamic_cast<CTile*>(pTile)->SetDrawID(iDrawID);
		dynamic_cast<CTile*>(pTile)->SetOption(iOption);
		pTile->SetPos(vPos);
		pTile->Initialize();

		m_vecTile.push_back(pTile);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Tile Load", L"¼º°ø", MB_OK);
}

const Vec2 CTileManager::MouseToTile(POINT ptMouse)
{
	int	x = ptMouse.x / TILECX;
	int y = ptMouse.y / TILECY;

	int		iIndex = y * TILEX + x;

	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
		return Vec2();

	return static_cast<CTile*>(m_vecTile[iIndex])->GetPos();
}
