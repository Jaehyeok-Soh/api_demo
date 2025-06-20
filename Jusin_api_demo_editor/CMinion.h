#pragma once
#include "CCharacter.h"

class CMinion final
	: public CCharacter
{
public:
	enum MINIONSTATE { IDLE, MOVE, ATTACK, END };
public:
	CMinion();
	~CMinion();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);

private:
	virtual void CreateWeapon() override;

	void    Motion_Change() override;

	LPCWSTR     GetStateName(MINIONSTATE eState);//Å×½ºÆ®
	void		SetFrameKey();

	void	MoveTile();

	void DebugTextOut(HDC _dc);

	void AttackPoc();
	void AttackInit();

	void ChaseNexus(bool _bIsInit);

private:
	MINIONSTATE               m_eCurState;
	MINIONSTATE               m_ePreState;
	wstring					  m_strFrameKey;
	Vec2					  m_vTatgetNexusTile;
};

