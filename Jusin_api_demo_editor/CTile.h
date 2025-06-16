#pragma once
#include "CObject.h"

class CTile
	: public CObject
{
public:
	CTile();
	virtual ~CTile() PURE;

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

public:
	void SetDrawID(TILETYPE _iDrawID) { m_iDrawID = _iDrawID; }
	const TILETYPE GetDrawID() const { return m_iDrawID; }

	void SetOption(int _iOption) { m_iOption = _iOption; }
	const int GetOption() const { return m_iOption; }

	void SetTeam(bool _bVal) { m_bTeam = _bVal; }
	const bool GetTeam() const { return m_bTeam; }

protected:
	TILETYPE m_iDrawID;
	int m_iOption;
	bool m_bTeam;
};

