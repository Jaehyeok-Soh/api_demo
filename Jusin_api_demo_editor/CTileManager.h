#pragma once
#include "CTile.h"

class CTileManager
{
private:
	CTileManager();
	~CTileManager();

public:
	void		Initialize();
	void		Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

public:
	void	Drawing_Tile(POINT ptMouse, int _iDrawID, int _iOption);
	CTile::TILETYPE	Peeking_Tile(POINT ptMouse);
	void	Save_Tile();
	void	Load_Tile();

public:
	static CTileManager* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTileManager;
		}

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CTileManager* m_pInstance;
	vector<CObject*>	m_vecTile;
};

