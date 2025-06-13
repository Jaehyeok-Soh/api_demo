#pragma once
#include "CCharacter.h"

class CTurret final
	: public CCharacter
{
public:
	CTurret();
	~CTurret();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;
};

