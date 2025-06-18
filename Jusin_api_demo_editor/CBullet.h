#pragma once
#include "CObject.h"
class CBullet
	: public CObject
{
public:
	CBullet();
	~CBullet();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;
};

