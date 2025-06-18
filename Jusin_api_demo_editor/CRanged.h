#pragma once
#include "CWeapon.h"
class CRanged
	: public CWeapon
{
public:
	CRanged();
	~CRanged();

public:
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

};

