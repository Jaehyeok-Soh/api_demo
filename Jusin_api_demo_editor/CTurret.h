#pragma once
#include "CTower.h"

class CTurret final
	: public CTower
{
public:
	CTurret();
	virtual ~CTurret();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

private:
	virtual void CreateWeapon() override;
	void AttackProc();
	void AttackInit();

private:
	const TCHAR* strFrameBlueTurret;
	const TCHAR* strFrameRedTurret;

	STATE m_eCurState;
};

