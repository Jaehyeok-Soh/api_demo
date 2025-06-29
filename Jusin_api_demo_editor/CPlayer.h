#pragma once
#include "CCharacter.h"

class CSkill;
class CPlayer final
	: public CCharacter
{
public:
	friend class CSkillSwordman;
	friend class CUltSwordman;
	friend class CSkillAcher;
	friend class CUltAcher;

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
	const STATUSINFO GetStatus() const { return m_tStatusInfo; }
	void SetHp(int _iVal) { m_tStatusInfo.m_iHp = _iVal; }

	void SetIsDead(bool _bVal) { m_bDead = _bVal; }

	const bool GetIsMine() const { return m_bIsMine; }
	void	SetIsMine(bool _bVal) { m_bIsMine = _bVal; }

	const bool GetIsHost() const { return m_bIsHost; }
	void	SetIsHost(bool _bVal) { m_bIsHost = _bVal; }

	const int GetTargetId() const { return m_iTargetId; }
	void	SetTargetId(int _iVal) { m_iTargetId = _iVal; }

	const STATE GetState() const { return m_eCurState; }
	void	SetState(STATE _eVal) { m_eCurState = _eVal; }

	const int GetNetId() const { return m_iNetId; }
	void	SetNetId(int _iVal) { m_iNetId = _iVal; }

	const wstring GetAccount() const { return m_strAccount; }
	void SetAccount(wstring _wstr) { m_strAccount = _wstr; }

	const Vec2	GetSpawnPos() const { return m_vSpawnPos; }
	void SetSpawnPos(Vec2 _vVal) { m_vSpawnPos = _vVal; }

	const Vec2	GetSpawnScroll() const { return m_vSpawnScroll; }
	void SetSpawnScroll(Vec2 _vVal) { m_vSpawnScroll = _vVal; }

	const JOB GetJob() const { return m_eJob; }
	void SetJob(JOB _eVal) { m_eJob = _eVal; }

	wstring		SetFrameKey();
	void		SetFrameStart(int _iFrameStart) { m_tFrame.iFrameStart = _iFrameStart; }
	void		SetDirection(int _iDir) { m_vMoveDir.x = _iDir; }

	void	ToDTO(bool isStart, bool isQuit, string winner);

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

	void	AttackProc();
	void	AttackInit();

	void	UpdateSkills();

	void	fromJson();

	void	DeadProc();

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
	COLORREF			m_rgbColor;

	wstring				m_strAccount;

	DWORD				m_fResPawnTime;
	DWORD				m_fDeadTimeCount;

	Vec2				m_vSpawnPos;
	Vec2				m_vSpawnScroll;
};

