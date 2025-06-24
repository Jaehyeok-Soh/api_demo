#include "pch.h"
#include "CBushTile.h"
#include "CSceneManager.h"
#include "CBush.h"

CBushTile::CBushTile()
{
}

CBushTile::~CBushTile()
{
	Release();
}

void CBushTile::Initialize()
{
	__super::Initialize();

	CObject* pObj = new CBush();
	auto bush = static_cast<CBush*>(pObj);
	bush->Initialize();
	bush->SetScale(Vec2(4.f, 4.f));
	bush->SetPos(m_vPos);
	bush->SetName(L"Bush");
	bush->SetDrawID(m_iDrawID);
	bush->SetOption(m_iOption);
	CSceneManager::GetInstance()->GetCurScene()->AddObject(bush, OBJ_BUSH);
}