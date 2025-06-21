#include "pch.h"
#include "CButton.h"
#include "CBmpManager.h"
#include "CSceneManager.h"
#include "CKeyManager.h"

CButton::CButton() : m_iDrawID(0)
{
}

CButton::~CButton()
{
	Release();
}

void CButton::Initialize()
{
	m_vScale.x = 150.f;
	m_vScale.y = 150.f;
}

int CButton::Update()
{
	__super::Update_Rect();

	return NOEVENT;
}

void CButton::Late_Update()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&m_tRect, ptMouse))
	{
		if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
		{
			if (!lstrcmp(L"ButtonPlay", m_pFrameKey))
			{
				CSceneManager::GetInstance()->SetChangeScene(true, SC_LOADING);
			}
			else if (!lstrcmp(L"ButtonEdit", m_pFrameKey))
			{
				CSceneManager::GetInstance()->SetChangeScene(true, SC_EDIT);
			}
			else if (!lstrcmp(L"ButtonExit", m_pFrameKey))
			{
				CSceneManager::GetInstance()->SetChangeScene(false, SC_MENU);
				DestroyWindow(g_hWnd);
			}

			return;
		}
		m_iDrawID = 1;
	}
	else
		m_iDrawID = 0;
}

void CButton::Render(HDC hDC)
{
	HDC	hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_vScale.x,
		(int)m_vScale.y,
		hMemDC,
		(int)m_vScale.x * m_iDrawID,
		0,
		(int)m_vScale.x,	// 복사할 비트맵 가로 세로 사이즈
		(int)m_vScale.y,
		RGB(255, 255, 255));	// 제거할 픽셀 색상 값
}

void CButton::Release()
{
}
