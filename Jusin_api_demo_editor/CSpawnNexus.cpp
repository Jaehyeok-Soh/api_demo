#include "pch.h"
#include "CSpawnNexus.h"
#include "CSceneManager.h"
#include "CNexus.h"

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
	nexus->SetScale(Vec2(64.f, 64.f));
	nexus->SetDrawID(m_iDrawID);
	nexus->SetOption(m_iOption);
	if (nexus->GetDrawID() == BLUE_NEXUS)
		nexus->SetPos(Vec2(m_vPos.x - 24, m_vPos.y + 24));
	else
		nexus->SetPos(Vec2(m_vPos.x + 24, m_vPos.y - 24));
	nexus->SetTeam(nexus->GetDrawID() == BLUE_NEXUS);
	CSceneManager::GetInstance()->GetCurScene()->AddObject(nexus, OBJ_TOWER);
}
