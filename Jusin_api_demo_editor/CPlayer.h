#pragma once
#include "CCharacter.h"

class CPlayer final
	: public CCharacter
{
public:
	enum STATE { IDLE, WALK, ATTACK, JUMP, ALERT, DEATH, LADDER, END };

public:
	CPlayer();
	~CPlayer();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

public:
	const bool GetIsMine() const { return m_bIsMine; }
	void SetIsMine(bool _bVal) { m_bIsMine = _bVal; }

	const bool GetIsHost() const { return m_bIsHost; }
	void SetIsHost(bool _bVal) { m_bIsHost = _bVal; }
	 
private:
	void    Key_Input();
	void    Offset();
	void    Motion_Change() override;

private:
	STATE               m_eCurState;
	STATE               m_ePreState;

	bool m_bIsMine;
	bool m_bIsHost;
};

