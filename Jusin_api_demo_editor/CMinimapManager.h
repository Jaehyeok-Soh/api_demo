#pragma once
#include "CPlayer.h"
class CMinimapManager
{
public:
	CMinimapManager();
	~CMinimapManager();

	CMinimapManager(CMinimapManager&) = delete;
	CMinimapManager(CMinimapManager&&) = delete;
	CMinimapManager& operator()(CMinimapManager&) = delete;
	CMinimapManager& operator()(CMinimapManager&&) = delete;

public:
	static CMinimapManager* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CMinimapManager();
		}
		return m_pInstance;
	}

	static void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void Initialize(CObject* _pObj);
	void Update();
	void Render(HDC _dc);

private:
	void EllipsePadding();
	void UpdateEllipse();
	void CalcPlayerPos();
	void DrawPlayer(HDC _dc);
	void DrawPath(HDC _dc);

	void KeyInput();

	void RectPadding();
	void UpdateScroll();
	void DrawRect(HDC _dc);

	void UpdateRect();

private:
	static CMinimapManager* m_pInstance;

	Vec2 m_vMapRatio;

	//스크롤 비례 위치
	Vec2 m_vPos;
	Vec2 m_vScale;
	RECT m_tRect;

	INFO m_tMapInfo;

	INFO m_tMiniMapInfo;

	CPlayer* m_pPlayer;
	//플레이어 비례 위치
	Vec2 m_vPlayerPos;
	RECT m_tPlayerRect;
};

