#pragma once
#include "CObject.h"
class CStartButton :
    public CObject
{
public:
    CStartButton();
    ~CStartButton();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _dc) override;
    void Release() override;

private:
    int m_iDrawID;
    HDC m_hMemDC;
};

