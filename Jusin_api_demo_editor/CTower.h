#pragma once
#include "CCharacter.h"

class CTower
	: public CCharacter
{
public:
	CTower();
	virtual ~CTower() PURE;

public:
	virtual void Initialize() override;
};
