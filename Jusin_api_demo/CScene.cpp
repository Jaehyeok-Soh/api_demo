#include "pch.h"
#include "CObject.h"
#include "CScene.h"
#include "CBmpManager.h"
#include "CScrollManager.h"

CScene::CScene() : m_pPlayer(nullptr), m_bFade(false)
{

}

CScene::~CScene()
{
	// 플레이어는 삭제하지 않음
	for (UINT i = 0; i < OBJ_END; ++i)
	{
		/*if (i == (UINT)OBJID::OBJ_PLAYER || i == (UINT)OBJID::OBJ_WEAPON || i == (UINT)OBJID::OBJ_GROUND)
			continue;*/
		if (i == OBJ_PLAYER)
			continue;

		for (CObject* pObj : m_arrObj[i])
		{
			Safe_Delete(pObj);
		}
		m_arrObj[i].clear();
	}
}

void CScene::Initialize()
{
	for (UINT i = 0; i < OBJ_END; ++i)
	{
		if (i == OBJ_PLAYER)
			continue;

		for (CObject* pObj : m_arrObj[i])
		{
			pObj->Initialize();
		}
	}
}

void CScene::Update()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_arrObj[i].begin();
			iter != m_arrObj[i].end(); )
		{
			int iResult = (*iter)->Update();

			if (DEAD == iResult)
			{
				Safe_Delete<CObject*>(*iter);
				iter = m_arrObj[i].erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}

void CScene::Late_Update()
{
	for (UINT i = 0; i < OBJ_END; ++i)
	{
		for (CObject* pObj : m_arrObj[i])
		{
			pObj->Late_Update();
		}
	}
}

void CScene::Render(HDC _dc)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (CObject* pObj : m_arrObj[i])
		{
			pObj->Render(_dc);
		}
	}
}

void CScene::DeleteGroup(OBJID _eTarget)
{
	auto& objList = m_arrObj[(UINT)_eTarget];

	for (CObject* pObj : objList)
	{
		delete pObj;
	}
	objList.clear();
}

void CScene::DeleteAll()
{
	for (UINT i = 0; i < OBJ_END; ++i)
	{
		if (i == OBJ_PLAYER)
			continue; // 플레이어는 삭제하지 않음

		/*if (i == (UINT)OBJID::OBJ_GROUND)
			continue;

		if (i == (UINT)OBJID::OBJ_WEAPON)
			continue;*/



		DeleteGroup((OBJID)i);
	}
}


void CScene::SetMaxScroll(const TCHAR* pImgKey)
{
	HBITMAP m_Map = CBmpManager::Get_Instance()->Get_HBitmap(pImgKey);

	if (m_Map)
	{
		BITMAP bmpInfo = {};
		GetObject(m_Map, sizeof(bmpInfo), &bmpInfo);

		CScrollManager::Get_Instance()->Set_ScrollXMax(bmpInfo.bmWidth);
		CScrollManager::Get_Instance()->Set_ScrollYMax(bmpInfo.bmHeight);

	}
}