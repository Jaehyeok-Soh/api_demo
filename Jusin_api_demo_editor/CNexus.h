#pragma once
#include "CTower.h"

class CNexus final
	: public CTower
{
public:
	CNexus();
	virtual ~CNexus();

	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

public:
	void SetSpawnPos(Vec2 _vVal) { m_vSpawnPos = _vVal; }

private:
	void SpawnMinion();

private:
	const TCHAR* strFrameBlueNexus;
	const TCHAR* strFrameRedNexus;

	float m_fSpawnCoolDownTime;

	Vec2 m_vSpawnPos;
	float m_fSpawnTime = 0.f;
	const float m_fSpawnDelay = 0.1f;
	int m_iSpawnCnt = 0;
	const int m_fSpawnMaxOnTime = 6;
};

