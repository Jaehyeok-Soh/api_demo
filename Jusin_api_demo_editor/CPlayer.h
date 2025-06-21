#pragma once
#include "CCharacter.h"

class CSkill;
class CPlayer final
	: public CCharacter
{
public:
	friend class CSkillSwordman;
	friend class CUltSwordman;

	enum JOB{SWORDMAN, ACHER, MAGICKNIGHT};

public:
	CPlayer();
	~CPlayer();

public:
	void	Initialize() override;
	int		Update() override;
	void	Late_Update() override;
	void	Render(HDC _dc) override;
	void	Release() override;

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);

	void	OnPeek(CObject* _pTargetObj);

public:
	const bool GetIsMine() const { return m_bIsMine; }
	void	SetIsMine(bool _bVal) { m_bIsMine = _bVal; }

	const bool GetIsHost() const { return m_bIsHost; }
	void	SetIsHost(bool _bVal) { m_bIsHost = _bVal; }

	const STATE GetState() const { return m_eCurState; }
	void	SetState(STATE _eVal) { m_eCurState = _eVal; }

	const int GetNetId() const { return m_iNetId; }
	void	SetNetId(int _iVal) { m_iNetId = _iVal; }

	wstring		SetFrameKey();
	void		SetFrameStart(int _iFrameStart) { m_tFrame.iFrameStart = _iFrameStart; }
	void		SetDirection(int _iDir) { m_vMoveDir.x = _iDir; }

private:
	void    Key_Input();
	void    Offset();
	void    Motion_Change() override;

	LPCWSTR     GetStateName(STATE eState);
	void		MakeFrameKey(const TCHAR* strJob, const TCHAR* strDir);

	void	MoveTile();

	void	DebugTextOut(HDC _dc);

	virtual void CreateWeapon() override;
	void	CreateSkill();

	void	AttackPoc();
	void	AttackInit();

	void	UpdateSkills();

	void	ToDTO();
	void	fromJson();

private:
	STATE               m_eCurState;
	STATE               m_ePreState;
	JOB					m_eJob;
	wstring				m_strFrameKey;
	vector<CSkill*>		m_vSkills;
	BMPSCALE			m_tBmpScale;
	float				m_fRenderScale;
	bool				m_bIsUsingSkill;
	int					m_iCurrentSkill;

	bool				m_bIsMine;
	bool				m_bIsHost;

	int					m_iNetId;

	float				m_fPlayTime;
};

