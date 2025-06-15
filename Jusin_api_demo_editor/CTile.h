#pragma once
#include "CObject.h"

class CTile
	: public CObject
{
public:
	enum TILETYPE
	{
		PEEK_DISABLE = 0,
		PEEK_ENABLE,
		TERRAIN_BUSH,

		BLUE_SP = 10,
		BLUE_NEXUS,
		BLUE_INHIBITER,
		BLUE_TURRET1,
		BLUE_TURRET2,
		BLUE_MELEE_SP,

		RED_SP = 20,
		RED_NEXUS,
		RED_INHIBITER,
		RED_TURRET1,
		RED_TURRET2,
		RED_MELEE_SP
	};
public:
	CTile();
	virtual ~CTile();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

public:
	void SetDrawID(int _iDrawID) { m_iDrawID = _iDrawID; }
	const int GetDrawID() const { return m_iDrawID; }

	void SetOption(int _iOption) { m_iOption = _iOption; }
	const int GetOption() const { return m_iOption; }

private:
	int m_iDrawID;
	int m_iOption;
};

