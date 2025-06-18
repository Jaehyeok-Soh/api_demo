#pragma once
#include "CObject.h"

class CHitbox
	: public CObject
{
public:
	CHitbox();
	~CHitbox();
public:
	void Initialize() override;
	void Initialize(HITBOXINFO _tVal);
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

public:
	void OnCollisionEnter(CCollider* _pOther) override;

private:
	HITBOXINFO m_tHitboxInfo;
};