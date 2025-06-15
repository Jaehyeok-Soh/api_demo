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
	pObj->Initialize();
	pObj->SetPos(m_vPos);
	pObj->SetScale(Vec2(64.f, 64.f));
	CSceneManager::GetInstance()->GetCurScene()->AddObject(pObj, OBJ_TOWER);
}
