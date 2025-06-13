#pragma once
#include "CObject.h"
class CUI
{
public:
	CUI();
	virtual ~CUI() PURE;

public:
	virtual void Initialize()	PURE;
	virtual void Update()		PURE;
	virtual void Late_Update()	PURE;
	virtual void Render()		PURE;
	virtual void Release()		PURE;
};

