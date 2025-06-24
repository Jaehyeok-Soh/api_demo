#include "pch.h"
#include "CSpawnNexus.h"
#include "CSceneManager.h"
#include "CNexus.h"
#include "CGameManager.h"

CSpawnNexus::CSpawnNexus()
{
}

CSpawnNexus::~CSpawnNexus()
{
}

void CSpawnNexus::Initialize()
{
	__super::Initialize();

	CObject* pObj = new CNexus();
	auto nexus = static_cast<CNexus*>(pObj);
	nexus->Initialize();
	nexus->SetScale(Vec2(24.f, 24.f));
	nexus->SetName(L"Nexus");
	nexus->SetDrawID(m_iDrawID);
	nexus->SetOption(m_iOption);
	if (nexus->GetDrawID() == BLUE_NEXUS)
		nexus->SetPos(Vec2(m_vPos.x - 10, m_vPos.y + 10));
	else
		nexus->SetPos(Vec2(m_vPos.x + 10, m_vPos.y - 10));
	nexus->SetSpawnPos(m_vPos);
	nexus->SetTeam(nexus->GetDrawID() == BLUE_NEXUS);
	CSceneManager::GetInstance()->GetCurScene()->AddObject(nexus, OBJ_TOWER);

	CGameManager::GetInstance()->AddSequenceObject(m_iDrawID, m_iOption);
}
