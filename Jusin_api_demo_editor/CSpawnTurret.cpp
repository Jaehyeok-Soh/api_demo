#include "pch.h"
#include "CSpawnTurret.h"
#include "CTurret.h"
#include "CSceneManager.h"
#include "CGameManager.h"

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
	auto turret = static_cast<CTurret*>(pObj);
	turret->Initialize();
	turret->SetPos(Vec2(m_vPos.x - 6, m_vPos.y));
	turret->SetScale(Vec2(24.f, 24.f));
	turret->SetName(L"Turret");
	turret->SetDrawID(m_iDrawID);
	turret->SetOption(m_iOption);
	turret->SetTeam(turret->GetDrawID() == BLUE_TURRET1 || turret->GetDrawID() == BLUE_TURRET2 || turret->GetDrawID() == BLUE_TURRET3);
	CSceneManager::GetInstance()->GetCurScene()->AddObject(turret, OBJ_TOWER);

	CGameManager::GetInstance()->AddSequenceObject(m_iDrawID, m_iOption);
}
