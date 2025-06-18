#pragma once
#include "CCharacter.h"

class CSkill;
class CPlayer final
	: public CCharacter
{
public:
	friend class CSkillSwordman;

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

	void OnPeek(CObject* _pTargetObj);

public:
	const bool GetIsMine() const { return m_bIsMine; }
	void SetIsMine(bool _bVal) { m_bIsMine = _bVal; }

	const bool GetIsHost() const { return m_bIsHost; }
	void SetIsHost(bool _bVal) { m_bIsHost = _bVal; }

	void SetState(STATE _eVal) { m_eCurState = _eVal; }
	 
private:
	void    Key_Input();
	void    Offset();
	void    Motion_Change() override;

	LPCWSTR     GetStateName(STATE eState);//�׽�Ʈ
	wstring		SetFrameKey();
	void		MakeFrameKey(const TCHAR* strJob, const TCHAR* strDir);

	void	MoveTile();

	void DebugTextOut(HDC _dc);

	virtual void CreateWeapon() override;
	void CreateSkill();

	void AttackPoc();
	void AttackInit();

private:
	STATE               m_eCurState;
	STATE               m_ePreState;
	JOB					m_eJob;
	wstring				m_strFrameKey;
	CSkill*				m_pSkill;

	bool m_bIsMine;
	bool m_bIsHost;

	float m_fPlayTime;
	
};

