#pragma once
#include "CTower.h"

class CNexus final
	: public CTower
{
public:
	CNexus();
	virtual ~CNexus();

	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

private:
	const TCHAR* strFrameBlueNexus;
	const TCHAR* strFrameRedNexus;
};

