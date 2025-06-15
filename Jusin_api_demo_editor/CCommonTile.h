#pragma once
#include "CTile.h"

class CCommonTile
	: public CTile
{
public:
	CCommonTile();
	~CCommonTile();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;
};

