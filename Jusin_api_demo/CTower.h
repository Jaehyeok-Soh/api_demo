#pragma once
#include "CCharacter.h"

class CTower final
	: public CCharacter
{
public:
	CTower();
	~CTower();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;
};

