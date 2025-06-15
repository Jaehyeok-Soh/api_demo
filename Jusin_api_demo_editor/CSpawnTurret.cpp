#include "pch.h"
#include "CSpawnTurret.h"
#include "CTurret.h"
#include "CSceneManager.h"

CSpawnTurret::CSpawnTurret()
{
}

CSpawnTurret::~CSpawnTurret()
{
}

void CSpawnTurret::Initialize()
{
	__super::Initialize();

	CObject* pObj = new CTurret();
	pObj->Initialize();
	pObj->SetPos(m_vPos);
	pObj->SetScale(Vec2(64.f, 64.f));
	CSceneManager::GetInstance()->GetCurScene()->AddObject(pObj, OBJ_TOWER);
}
