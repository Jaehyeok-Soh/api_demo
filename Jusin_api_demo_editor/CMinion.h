#pragma once
#include "CCharacter.h"

class CMinion final
	: public CCharacter
{
public:
	enum MINIONSTATE { MOVE, ATTACK, END };
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

	void	MoveVector();

	void DebugTextOut(HDC _dc);

private:
	MINIONSTATE               m_eCurState;
	MINIONSTATE               m_ePreState;
	wstring				m_strFrameKey;
	deque<Vec2>			m_Path;
};

