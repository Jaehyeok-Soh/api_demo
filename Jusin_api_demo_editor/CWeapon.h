#pragma once
#include "CObject.h"

class CWeapon
	: public CObject
{
public:
	CWeapon();
	~CWeapon();

	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

private:
	CObject* pOwner;
};

