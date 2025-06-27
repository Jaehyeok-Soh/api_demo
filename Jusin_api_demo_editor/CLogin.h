#pragma once
#include "CScene.h"
#include "CLoginButton.h"

class CLogin
	: public CScene
{
public:
	CLogin();
	~CLogin();

public:
	void Initialize();
	void Update() override;
	void Render(HDC _dc) override;
	void Enter() override;
	void Exit() override;

public:
	wstring GetAccountText();

private:
	void Render_Map(HDC hdc);
	void CreateTextInputHandle();

private:
	HWND		m_hChildHandle;
	wstring m_wstrInput;
	CLoginButton* m_LoginButton;
};