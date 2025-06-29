#pragma once
#include "CObject.h"
class CWall
	: public CObject
{
public:
	CWall();
	~CWall();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
};

