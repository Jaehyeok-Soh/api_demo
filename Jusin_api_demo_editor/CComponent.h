#pragma once

class CComponent
{
	friend class CObject;
public:
	CComponent();
	virtual ~CComponent() PURE;

public:
	void Initialize(CObject* _pOwner);
	int Update();

public:
	CObject* GetObj() { return m_pOwner; }

	void	SetActivate(bool _bVal) { m_bActive = _bVal; }
	bool	IsActive() { return m_bActive; }

protected:
	CObject* m_pOwner;

	bool m_bActive; // 활성화 여부
};