#pragma once
#include "CObject.h"
class CEogButton final :
    public CObject
{
public:
    CEogButton();
    ~CEogButton();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC _dc) override;
    void Release() override;
};

