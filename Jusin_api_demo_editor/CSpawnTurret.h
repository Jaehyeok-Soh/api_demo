#pragma once
#include "CSpawnTile.h"
class CSpawnTurret :
    public CSpawnTile
{
public:
    CSpawnTurret();
    ~CSpawnTurret();

public:
    void Initialize() override;
};

