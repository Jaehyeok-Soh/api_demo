#pragma once
#include "CObject.h"

class CEffect
	: public CObject
{
public:
	CEffect();
	~CEffect();

public:
	void Initialize() override;
	void Initialize(FRAME _fFrame, BMPSCALE _tScale, const TCHAR* _pFrameKey);
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

private:
	BMPSCALE m_tBmpScale;
};

