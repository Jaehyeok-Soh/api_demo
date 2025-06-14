#include "pch.h"
#include "CScrollManager.h"

CScrollManager* CScrollManager::m_pInstance = nullptr;

CScrollManager::CScrollManager()
	: m_fScrollX(0.f), m_fScrollY(0.f)
{
}

CScrollManager::~CScrollManager()
{
}

void CScrollManager::Set_ScrollX(float fX)
{
    m_fScrollX += fX;

    if (m_fScrollX > 0.f)
        m_fScrollX = 0.f;

    //MAP_WIDTH - WINCX는 화면에 보이지 않는 나머지 우측 맵의 크기.
    if (m_fScrollX < -(m_fScrollXMax - (WINCX / g_fZoom)))
        m_fScrollX = -(m_fScrollXMax - (WINCX / g_fZoom));
}

void CScrollManager::Set_ScrollY(float fY)
{
    m_fScrollY += fY;

    if (m_fScrollY > 0.f)
        m_fScrollY = 0.f;

    if (m_fScrollY < -(m_fScrollYMax - (WINCY / g_fZoom)))
        m_fScrollY = -(m_fScrollYMax - (WINCY / g_fZoom));
}