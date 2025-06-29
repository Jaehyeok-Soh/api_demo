#include "pch.h"
#include "CEogButton.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CBmpManager.h"

CEogButton::CEogButton()
	: m_iDrawID(0),
	m_hMemDC(0)
{
}

CEogButton::~CEogButton()
{
	Release();
}

void CEogButton::Initialize()
{
	m_vScale.x = 152.f;
	m_vScale.y = 38.f;

	m_pFrameKey = L"eogButton";
}

int CEogButton::Update()
{
	__super::Update_Rect();

	return NOEVENT;
}

void CEogButton::Late_Update()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&m_tRect, ptMouse))
	{
		SetCursor(g_hHandCursor);
		m_iDrawID = 1;
		if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
		{
			if (!lstrcmp(L"eogButton", m_pFrameKey))
			{
				m_iDrawID = 3;
				CSceneManager::GetInstance()->SetChangeScene(true, SC_LOBBY);
			}
		}
		return;
	}
	else
		m_iDrawID = 0;
}

void CEogButton::Render(HDC _dc)
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
		(int)m_vScale.x,
		(int)m_vScale.y,
		RGB(0, 0, 0));
}

void CEogButton::Release()
{
}
