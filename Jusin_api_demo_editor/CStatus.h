#pragma once
#include "CUI.h"
class CStatus
	: public CUI
{
public:
	CStatus();
	~CStatus();

public:
	void Initialize() override;
	void Update() override;
	void Late_Update() override;
	void Render() override;
	void Release() override;
};

