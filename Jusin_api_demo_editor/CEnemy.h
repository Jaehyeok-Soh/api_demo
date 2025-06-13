#pragma once
#include "CCharacter.h"

class CEnemy final
	: public CCharacter
{
public:
	CEnemy();
	~CEnemy();

	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;
};

