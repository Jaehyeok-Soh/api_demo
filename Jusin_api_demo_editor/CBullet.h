#pragma once
#include "CObject.h"
class CBullet
	: public CObject
{
public:
	CBullet();
	~CBullet();

public:
	void Initialize() override;
	void Initialize(CObject* _pTarget);
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

private:
	void CalcAngle();
	void MoveToAngle();

private:
	float m_fAngle;
	int m_iDamage;
};