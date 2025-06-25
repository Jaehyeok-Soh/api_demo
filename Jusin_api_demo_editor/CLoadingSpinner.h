#pragma once
#include "CObject.h"

class CLoadingSpinner
	: public CObject
{
public:
	CLoadingSpinner();
	~CLoadingSpinner();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

private:
	void Frame_Update_Spinner();

private:
	HDC   m_hMemDC;
};