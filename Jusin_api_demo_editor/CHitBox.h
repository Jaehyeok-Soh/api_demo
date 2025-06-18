#pragma once
#include "CObject.h"

class CHitbox
	: public CObject
{
public:
	struct HITBOXINFO
	{
		float m_fDuration;
		float m_fElapsed;
		int m_iDamage;
		bool m_bOnce;
		bool m_bHitApplied;
		CObject* m_pOwner;
	};

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