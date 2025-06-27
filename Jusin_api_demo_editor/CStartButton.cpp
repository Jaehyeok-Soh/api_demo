#include "pch.h"
#include "CStartButton.h"
#include "CBmpManager.h"
#include "CKeyManager.h"
#include "CSceneManager.h"

CStartButton::CStartButton()
	: m_iDrawID(0),
	m_hMemDC(0)
{
}

CStartButton::~CStartButton()
{
	Release();
}

void CStartButton::Initialize()
{
	m_vScale.x = 156.f;
	m_vScale.y = 40.f;
	
	m_pFrameKey = L"startButton";
}

int CStartButton::Update()
{
	__super::Update_Rect();

	return NOEVENT;
}

void CStartButton::Late_Update()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (m_iDrawID != 2)
	{
		if (PtInRect(&m_tRect, ptMouse))
		{
			SetCursor(g_hHandCursor);
			m_iDrawID = 1;
			if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
			{
				if (!lstrcmp(L"startButton", m_pFrameKey))
				{
					m_iDrawID = 2;
					//CSceneManager::GetInstance()->SetChangeScene(true, SC_WAITROOM);
					CSceneManager::GetInstance()->SetChangeScene(true, SC_LOADING);
				}
				else if (!lstrcmp(L"startButton3", m_pFrameKey))
				{
					m_iDrawID = 2;
					CSceneManager::GetInstance()->SetChangeScene(true, SC_LOADING);
				}
			}
			return;
		}
		else
			m_iDrawID = 0;
	}
}

void CStartButton::Render(HDC _dc)
{
	m_hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);
	GdiTransparentBlt(_dc,
		m_tRect.left,
		m_tRect.top,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_hMemDC,
		(int)m_vScale.x * m_iDrawID,
		0,
		(int)m_vScale.x,	// 복사할 비트맵 가로 세로 사이즈
		(int)m_vScale.y,
		RGB(1, 1, 1));	// 제거할 픽셀 색상 값
}

void CStartButton::Release()
{
}
