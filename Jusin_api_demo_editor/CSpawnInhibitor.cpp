#include "pch.h"
#include "CSpawnInhibitor.h"
#include "CInhibitor.h"
#include "CSceneManager.h"

CSpawnInhibitor::CSpawnInhibitor()
{
}

CSpawnInhibitor::~CSpawnInhibitor()
{
}

void CSpawnInhibitor::Initialize()
{
	__super::Initialize();

	CObject* pObj = new CInhibitor();
	auto inhibitor = static_cast<CInhibitor*>(pObj);
	inhibitor->Initialize();
	inhibitor->SetScale(Vec2(32.f, 32.f));
	inhibitor->SetPos(Vec2(m_vPos.x - 8, m_vPos.y + 8));
	inhibitor->SetDrawID(m_iDrawID);
	inhibitor->SetOption(m_iOption);
	inhibitor->SetTeam(inhibitor->GetDrawID() == BLUE_INHIBITER);
	CSceneManager::GetInstance()->GetCurScene()->AddObject(inhibitor, OBJ_TOWER);
}
