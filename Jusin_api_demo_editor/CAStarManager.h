#pragma once
#include "CObject.h"

class CAStarManager
{
private:
	CAStarManager();
	~CAStarManager();

	CAStarManager(CAStarManager&) = delete;
	CAStarManager(CAStarManager&&) = delete;
	CAStarManager& operator()(CAStarManager&) = delete;
	CAStarManager& operator()(CAStarManager&&) = delete;

public:
	static CAStarManager* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CAStarManager();
		}
		return m_pInstance;
	}

	static void DestoryInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	deque<Vec2> FindPath(Vec2 start, Vec2 end);

private:
	bool IsWalkable(int _nx, int _ny);

private:
	static CAStarManager* m_pInstance;
	vector<CObject*> m_vecTile;

	int iCntX;
	int iCntY;

	int iMaxX;
	int iMaxY;
};

