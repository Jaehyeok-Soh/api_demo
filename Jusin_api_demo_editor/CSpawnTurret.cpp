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
	auto turret = static_cast<CTurret*>(pObj);
	turret->Initialize();
	turret->SetPos(Vec2(m_vPos.x - 8, m_vPos.y + 8));
	turret->SetScale(Vec2(32.f, 32.f));
	turret->SetName(L"Turret");
	turret->SetDrawID(m_iDrawID);
	turret->SetOption(m_iOption);
	turret->SetTeam(turret->GetDrawID() == BLUE_TURRET1 || turret->GetDrawID() == BLUE_TURRET2);
	CSceneManager::GetInstance()->GetCurScene()->AddObject(turret, OBJ_TOWER);
}
