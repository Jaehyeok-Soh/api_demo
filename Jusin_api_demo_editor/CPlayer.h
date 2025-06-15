#pragma once
#include "CCharacter.h"

class CPlayer final
	: public CCharacter
{
public:
	enum JOB{SWORDMAN, ACHER, MAGICKNIGHT};
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
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);

public:
	const bool GetIsMine() const { return m_bIsMine; }
	void SetIsMine(bool _bVal) { m_bIsMine = _bVal; }

	const bool GetIsHost() const { return m_bIsHost; }
	void SetIsHost(bool _bVal) { m_bIsHost = _bVal; }
	 
private:
	void    Key_Input();
	void    Offset();
	void    Motion_Change() override;

	LPCWSTR     GetStateName(STATE eState);//Å×½ºÆ®
	wstring		SetFrameKey();
	void		MakeFrameKey(const TCHAR* strJob, const TCHAR* strDir);

	void	MoveVector();

	void DebugTextOut(HDC _dc);

private:
	STATE               m_eCurState;
	STATE               m_ePreState;
	JOB					m_eJob;
	wstring				m_strFrameKey;
	deque<Vec2>			m_Path;

	bool m_bIsMine;
	bool m_bIsHost;
};

