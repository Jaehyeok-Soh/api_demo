#pragma once
#include "CTower.h"

class CInhibitor final
	: public CTower
{
public:
	CInhibitor();
	virtual ~CInhibitor();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

private:
	virtual void CreateWeapon() override;

private:
	const TCHAR* strFrameBlueInhibitor;
	const TCHAR* strFrameRedInhibitor;
};

