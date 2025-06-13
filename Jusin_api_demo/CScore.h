#pragma once
#include "CUI.h"
class CScore
	: public CUI
{
public:
	CScore();
	~CScore();

public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render() override;
	void Release() override;
};

