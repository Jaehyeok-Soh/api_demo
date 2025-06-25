#pragma once
#include "CObject.h"
class CLoginButton
	: public CObject
{
public:
	CLoginButton();
	~CLoginButton();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

public:
	void SetLoginEnable(bool _bVal) { m_bEnableLogin = _bVal; }

private:
	void GetAccount();

private:
	int m_iDrawID;
	HDC m_hMemDC;
	bool m_bEnableLogin;
};

