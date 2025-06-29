#pragma once
#include "CTile.h"
class CWallTile
	: public CTile
{
public:
	CWallTile();
	~CWallTile();

public:
	void Initialize() override;
};

