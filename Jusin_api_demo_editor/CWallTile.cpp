#include "pch.h"
#include "CWallTile.h"
#include "CWall.h"
#include "CSceneManager.h"

CWallTile::CWallTile()
{
}

CWallTile::~CWallTile()
{
	Release();
}

void CWallTile::Initialize()
{
	__super::Initialize();

	CObject* pObj = new CWall();
	auto bush = static_cast<CWall*>(pObj);
	bush->Initialize();
	bush->SetScale(Vec2(4.f, 4.f));
	bush->SetPos(m_vPos);
	bush->SetName(L"Wall");
	bush->SetDrawID(m_iDrawID);
	bush->SetOption(m_iOption);
	CSceneManager::GetInstance()->GetCurScene()->AddObject(bush, OBJ_WALL);
}
