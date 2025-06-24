#pragma once
#include "CTile.h"

class CBushTile final
	: public CTile
{
public:
	CBushTile();
	~CBushTile();

public:
	void Initialize() override;
};

