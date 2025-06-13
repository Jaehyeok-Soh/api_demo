#pragma once
#include "CObject.h"

class CTile
	: public CObject
{
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
	void SetOption(int _iOption) { m_iOption = _iOption; }

	const int GetDrawID() const { return m_iDrawID; }
	const int GetOption() const { return m_iOption; }

private:
	int m_iDrawID;
	int m_iOption;
};

