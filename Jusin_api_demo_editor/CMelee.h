#pragma once
#include "CWeapon.h"
class CMelee final
	: public CWeapon
{
public:
	CMelee();
	~CMelee();

public:
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

public:
	void Attack() override;
};

