#pragma once
#include "CObject.h"

class CBush final
	: public CObject
{
public:
	CBush();
	~CBush();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

public:
	virtual void OnCollision(CCollider* _pOther) override;
	virtual void OnCollisionEnter(CCollider* _pOther) override;
	virtual void OnCollisionExit(CCollider* _pOther) override;
};

