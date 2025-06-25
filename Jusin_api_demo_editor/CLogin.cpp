#include "pch.h"
#include "CLogin.h"
#include "CBmpManager.h"
#include "CLoginButton.h"

#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")

LRESULT CALLBACK EditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam,
	UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (message)
	{
	case WM_GETDLGCODE:
		return DLGC_WANTALLKEYS;  // 모든 키를 부모에게도 보내겠다!

	case WM_KEYDOWN:
		if (wParam == VK_RETURN)
		{
			HWND hParent = GetParent(hWnd);
			PostMessage(hParent, WM_KEYDOWN, wParam, lParam);
			return 0;  // 엔터를 Edit Control 내부에서 소비하지 않음
		}
		else if (wParam == VK_ESCAPE)
		{
			HWND hParent = GetParent(hWnd);
			PostMessage(hParent, WM_KEYDOWN, wParam, lParam);
			return 0;
		}
		break;
	}
	return DefSubclassProc(hWnd, message, wParam, lParam);
}

CLogin::CLogin()
{
}

CLogin::~CLogin()
{
}

void CLogin::Initialize()
{
	MapDC = CBmpManager::Get_Instance()->Find_Image(L"loginBg");

	CObject* pButtonLogin = new CLoginButton();
	pButtonLogin->Initialize();
	pButtonLogin->SetPos(Vec2(200.f, 534.5f));
	pButtonLogin->SetName(L"UI");
	pButtonLogin->Set_FrameKey(L"loginButton");
	AddObject(pButtonLogin, OBJ_UI);
	
	m_LoginButton = static_cast<CLoginButton*>(pButtonLogin);

	CreateTextInputHandle();
}

void CLogin::Update()
{
	if (GetWindowTextLength(m_hChildHandle) > 0)
		m_LoginButton->SetLoginEnable(true);
	else
		m_LoginButton->SetLoginEnable(false);

	CScene::Update();
}

void CLogin::Render(HDC _dc)
{
	Render_Map(_dc);

	CScene::Render(_dc);
}

void CLogin::Enter()
{
	Initialize();
}

void CLogin::Exit()
{
	DeleteAll();
	DestroyWindow(m_hChildHandle);
}

void CLogin::Render_Map(HDC hdc)
{
	GdiTransparentBlt(hdc,
		0,
		0,
		WINCX,
		WINCY,
		MapDC,
		0,
		40,
		1280,	// 복사할 비트맵 가로 세로 사이즈
		680,//nice
		RGB(1, 1, 1));	// 제거할 픽셀 색상 값
}

void CLogin::CreateTextInputHandle()
{
	RECT rc = { 65.f, 205.f, 0, 0 };
	m_hChildHandle = CreateWindowEx(
		0,
		L"EDIT",
		NULL,
		WS_CHILD | WS_VISIBLE | ES_LEFT,
		rc.left, rc.top, 270, 30,
		g_hWnd,
		(HMENU)1001,
		hInst,
		NULL
	);

	SetWindowSubclass(m_hChildHandle, EditProc, 0, 0);
}

wstring CLogin::GetAccountText()
{
	int iLength = GetWindowTextLength(m_hChildHandle);
	m_wstrInput = wstring(iLength, L'\0');
	GetWindowText(m_hChildHandle, &m_wstrInput[0], iLength + 1);
	m_wstrInput.resize(iLength);

	return m_wstrInput;
}
