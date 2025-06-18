#pragma once
#include "CSkill.h"
class CUltSwordman :
    public CSkill
{
public:
    CUltSwordman();
    ~CUltSwordman();

public:
    void Update(CPlayer& _pPlayer) override;
    void Initialize(CPlayer& _pPlayer) override;
    void Use(CPlayer& _pPlayer, int _iDir) override;
    bool IsFinished(CPlayer& _pPlayer, int _iDir) override;
};

