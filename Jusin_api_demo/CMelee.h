#pragma once
#include "CCharacter.h"

class CMelee final
	: public CCharacter
{
public:
	CMelee();
	~CMelee();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;
};

