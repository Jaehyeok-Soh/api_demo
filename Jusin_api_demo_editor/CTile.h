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

		//∫¿¿Œ
		BLUE1_SP = 10,
		BLUE1_NEXUS,
		BLUE1_INHIBITER,
		BLUE1_TURRET1,
		BLUE1_TURRET2,
		//∫¿¿Œ
		RED1_SP = 20,
		RED1_NEXUS,
		RED1_INHIBITER,
		RED1_TURRET1,
		RED1_TURRET2,
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

