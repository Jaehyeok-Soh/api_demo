#include "pch.h"
#include "CObject.h"

CObject::CObject() : m_fSpeed(0.f), m_bDead(false), m_fAngle(0.f), m_vVelocity{}, m_fDistance(0.f), m_vPos{}, m_vScale{}
, m_pTarget(nullptr), m_pCollider(nullptr),m_pFrameKey(L"")//, m_pGravity(nullptr)
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
	return 0;
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
	/*if (nullptr != m_pGravity)
	{
		Safe_Delete(m_pGravity);
	}*/
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

void CObject::CreateColider()
{
	if (!m_pCollider)
		m_pCollider = new CCollider;

	m_pCollider->m_pOwner = this;
}

//void CObject::CreateGravity()
//{
//	if (!m_pGravity)
//		m_pGravity = new CGravity;
//
//	m_pGravity->m_pOwner = this;
//}

void CObject::Component_Render(HDC _dc)
{
	if (nullptr != m_pCollider)
	{
		m_pCollider->Render(_dc);
	}
}