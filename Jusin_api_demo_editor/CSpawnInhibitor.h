#pragma once
#include "CSpawnTile.h"
class CSpawnInhibitor :
    public CSpawnTile
{
public:
    CSpawnInhibitor();
    ~CSpawnInhibitor();

public:
    void Initialize();
};

