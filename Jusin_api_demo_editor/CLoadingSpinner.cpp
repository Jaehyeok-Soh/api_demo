#include "pch.h"
#include "CLoadingSpinner.h"
#include "CBmpManager.h"

CLoadingSpinner::CLoadingSpinner()
    : m_hMemDC(0)
{
}

CLoadingSpinner::~CLoadingSpinner()
{
}

void CLoadingSpinner::Initialize()
{
    //loadingscreen_spinner
    m_vScale = Vec2(32.f, 32.f);
    m_pFrameKey = L"loadingscreen_spinner";

    m_hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);

    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 7;
    m_tFrame.iMotion = 0;
    m_tFrame.dwTime = GetTickCount();
    m_tFrame.dwSpeed = 50;
}

int CLoadingSpinner::Update()
{
    __super::Update_Rect();

    Frame_Update_Spinner();

    return NOEVENT;
}

void CLoadingSpinner::Late_Update()
{
}

void CLoadingSpinner::Render(HDC _dc)
{
    GdiTransparentBlt(_dc,
        m_tRect.left,
        m_tRect.top,
        m_vScale.x,
        m_vScale.y,
        m_hMemDC,
        m_vScale.x * m_tFrame.iFrameStart,
        m_vScale.y * m_tFrame.iMotion,
        m_vScale.x,   // 복사할 비트맵 가로 세로 사이즈
        m_vScale.y,
        RGB(255, 255, 255));   // 제거할 픽셀 색상 값
}

void CLoadingSpinner::Frame_Update_Spinner()
{
    if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
    {
        ++m_tFrame.iFrameStart;
        m_tFrame.dwTime = GetTickCount();
    }

    if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
    {
        m_tFrame.iFrameStart = 0;
        m_tFrame.iMotion++;

        if (m_tFrame.iMotion > 3)
        {
            m_tFrame.iMotion = 0;
        }
    }
}

void CLoadingSpinner::Release()
{
}
