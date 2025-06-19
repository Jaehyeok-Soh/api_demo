#include "pch.h"
#include "CObject.h"
#include "CAStarManager.h"
#include "CSceneManager.h"
#include "CScrollManager.h"

UINT CObject::m_iNextObjectId = 0;

CObject::CObject() 
	: m_fSpeed(0.f),
	m_bDead(false),
	m_fAngle(0.f),
	m_vVelocity{},
	m_fDistance(0.f),
	m_vPos{}, m_vScale{},
	m_pTarget(nullptr),
	m_pCollider(nullptr),
	m_pFrameKey(L""),
	m_bTeam(true),
	m_eDrawID(TILETYPE::BLUE_INHIBITER),
	m_iOption(1),
	m_iObjectId(m_iNextObjectId++)
	//, m_pGravity(nullptr)
{
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CObject::CObject(const CObject& _origin) : m_strName(_origin.m_strName), m_vPos(_origin.m_vPos), m_vScale(_origin.m_vScale), m_bDead(_origin.m_bDead)//, m_pColider(nullptr)
{
	if (_origin.m_pCollider)
	{
		m_pCollider = new CCollider(*_origin.m_pCollider);
		m_pCollider->m_pOwner = this;
	}
	/*if (_origin.m_pGravity)
	{
		m_pGravity = new CGravity(*_origin.m_pGravity);
		m_pGravity->m_pOwner = this;
	}*/
}

CObject::~CObject()
{
	Release();
}

void CObject::Initialize()
{
}

int CObject::Update()
{
	return NOEVENT;
}

void CObject::Late_Update()
{
}

void CObject::Render(HDC _dc)
{
}

void CObject::Release()
{
	if (nullptr != m_pCollider)
	{
		Safe_Delete(m_pCollider);
	}
}

float CObject::Get_DistToTarget()
{
	float fWidth = m_vPos.x - m_pTarget->GetPos().x;
	float fHeight = m_vPos.y - m_pTarget->GetPos().y;

	return sqrt(fWidth * fWidth + fHeight * fHeight);
}

float CObject::Get_Dist(CObject* _pObj)
{
	float fWidth = m_vPos.x - _pObj->GetPos().x;
	float fHeight = m_vPos.y - _pObj->GetPos().y;

	return sqrt(fWidth * fWidth + fHeight * fHeight);
}

void CObject::ChaseTarget()
{
	m_Path.clear();

	Vec2 startIdx = Vec2(m_vPos.x / TILECX, m_vPos.y / TILECY);
	Vec2 endIdx = TargetPosToTile();

	auto future = async(launch::async,
		&CAStarManager::FindPath,
		CAStarManager::GetInstance(),
		startIdx, endIdx);

	m_Path = future.get();

	if (!m_Path.empty())
	{
		m_Path.pop_front();
	}
}

void CObject::FindTarget()
{
	CObject* closedObject = nullptr;
	float fTargetDist = 99999999.f;
	auto objectList = CSceneManager::GetInstance()->GetCurScene()->GetObjectList();
	
	for (auto minion : objectList[OBJ_MINION])
	{
		if (minion->GetTeam() == m_bTeam)
			continue;

		float cmpDist = Get_Dist(minion);
		if (fTargetDist > cmpDist)
		{
			closedObject = minion;
			fTargetDist = cmpDist;
		}
	}

	if (GetCollider()->Get_Layer() == COL_MINION)
	{
		for (auto tower : objectList[OBJ_TOWER])
		{
			if (tower->GetTeam() == m_bTeam)
				continue;

			float cmpDist = Get_Dist(tower);
			if (fTargetDist > cmpDist)
			{
				closedObject = tower;
				fTargetDist = cmpDist;
			}
		}
	}

	for (auto otherPlayer : objectList[OBJ_PLAYER])
	{
		if (otherPlayer->GetTeam() == m_bTeam)
			continue;

		float cmpDist = Get_Dist(otherPlayer);
		if (fTargetDist > cmpDist)
		{
			closedObject = otherPlayer;
			fTargetDist = cmpDist;
		}
	}

	if (fTargetDist > 50.f)
	{
		m_pTarget = nullptr;
		m_bOnTarget = false;
	}
	else
	{
		m_pTarget = closedObject;
		m_bOnTarget = true;
	}
}

Vec2 CObject::TargetPosToTile()
{
	Vec2 vtargetPos = m_pTarget->GetPos();
	Vec2 endIdx = Vec2(vtargetPos.x / TILECX, vtargetPos.y / TILECY);

	return endIdx;
}

void CObject::Update_Rect()
{
	m_tRect.left = LONG(m_vPos.x - (m_vScale.x / 2.f));
	m_tRect.top = LONG(m_vPos.y - (m_vScale.y / 2.f));
	m_tRect.right = LONG(m_vPos.x + (m_vScale.x / 2.f));
	m_tRect.bottom = LONG(m_vPos.y + (m_vScale.y / 2.f));
}

void CObject::Update_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;
	}
}

void CObject::Update_Frame_Reverse()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		--m_tFrame.iFrameStart;
		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iFrameStart < m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = m_tFrame.iStartBuffer;
	}
}

void CObject::CreateCollider()
{
	if (!m_pCollider)
		m_pCollider = new CCollider;

	m_pCollider->m_pOwner = this;
}

void CObject::Component_Render(HDC _dc)
{
	if (nullptr != m_pCollider)
	{
		m_pCollider->Render(_dc);
	}
}