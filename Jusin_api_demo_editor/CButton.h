#pragma once
#include "CObject.h"

class CButton
	: public CObject
{
public:
	CButton();
	virtual ~CButton();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	int		m_iDrawID;

};

