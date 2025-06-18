#pragma once
#include "CObject.h"

class CEffect
	: public CObject
{
public:
	CEffect();
	virtual ~CEffect() PURE;

private:
	virtual void Motion_Change() {};
};

