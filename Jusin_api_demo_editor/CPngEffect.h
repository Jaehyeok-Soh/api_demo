#pragma once
#include "CObject.h"
class CPngEffect :
    public CObject
{
public:
	CPngEffect();
	~CPngEffect();

public:
	void Initialize() override;
	void Initialize(FRAME _fFrame, BMPSCALE _tScale, const TCHAR* _pFrameKey, COLORREF _rgb);
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

private:
	BMPSCALE m_tBmpScale;
	COLORREF m_rgbColor;
};

