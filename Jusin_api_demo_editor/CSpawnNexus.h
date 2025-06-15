#pragma once
#include "CSpawnTile.h"
class CSpawnNexus :
    public CSpawnTile
{
public:
    CSpawnNexus();
    ~CSpawnNexus();

public:
    void Initialize() override;
};