#include "pch.h"
#include "CLoginButton.h"
#include "CBmpManager.h"
#include "CSceneManager.h"
#include "CKeyManager.h"
#include "CLogin.h"
#include "CGameManager.h"

CLoginButton::CLoginButton()
	: m_iDrawID(1),
	m_bEnableLogin(false),
	m_hMemDC(0)
{
}

CLoginButton::~CLoginButton()
{
	Release();
}

void CLoginButton::Initialize()
{
	m_vScale.x = 64.f;
	m_vScale.y = 64.f;

	m_pFrameKey = L"loginButton";
	m_hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);
}

int CLoginButton::Update()
{
	__super::Update_Rect();

	return NOEVENT;
}

void CLoginButton::Late_Update()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (m_bEnableLogin)
	{
		m_iDrawID = 1;

		if (CKeyManager::Get_Instance()->Key_Pressing(VK_RETURN))
		{
			GetAccount();
			CSceneManager::GetInstance()->SetChangeScene(true, SC_LOBBY);
			return;
		}

		if (PtInRect(&m_tRect, ptMouse))
		{
			SetCursor(g_hHandCursor);
			if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
			{
				if (!lstrcmp(L"loginButton", m_pFrameKey))
				{
					GetAccount();
					CSceneManager::GetInstance()->SetChangeScene(true, SC_LOBBY);
				}
			}
			return;
		}
	}
	else
		m_iDrawID = 0;
}

void CLoginButton::Render(HDC _dc)
{
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
		RGB(255, 0, 255));	// 제거할 픽셀 색상 값
}

void CLoginButton::Release()
{
}

void CLoginButton::GetAccount()
{
	auto loginScene = static_cast<CLogin*>(CSceneManager::GetInstance()->GetCurScene());
	auto a = loginScene->GetAccountText();
	CGameManager::GetInstance()->SetAccount(a);
}
