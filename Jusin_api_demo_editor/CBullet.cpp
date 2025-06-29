#include "pch.h"
#include "CBullet.h"
#include "CTimeManager.h"
#include "CScrollManager.h"
#include "CBmpManager.h"
#include "CWeapon.h"
#include "CRanged.h"
#include "CBlendingManager.h"

CBullet::CBullet()
	: pBulletWeapon(nullptr)
{
}

CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize()
{
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 60;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 5;

	m_fSpeed = 150.f;
}

void CBullet::Initialize(CObject* _pTarget)
{
	Initialize();

	m_pTarget = _pTarget;

	CreateCollider();

	GetCollider()->SetScale(Vec2(8.f, 8.f));
	GetCollider()->Set_Layer(COL_ATTACK);
	GetCollider()->Set_Mask(COL_MINION
		| COL_TOWER
		| COL_PLAYER);
}

int CBullet::Update()
{
	if (static_cast<CRanged*>(pBulletWeapon)->GetOwner()->Get_Dead()
		|| static_cast<CRanged*>(pBulletWeapon)->GetOwner() == nullptr)
		Set_Dead();
	
	if (m_pTarget->Get_Dead())
		Set_Dead();

	if (m_bDead)
		return DEAD;

	CalcAngle();
	MoveToAngle();

	__super::Update_Rect();

	__super::Update_Frame();

	return NOEVENT;
}

void CBullet::Late_Update()
{
	if (m_pCollider)
		m_pCollider->Late_Update();
}

void CBullet::Render(HDC _dc)
{
	if (m_vMoveDir.x > 0)
	{
		if (pBulletWeapon->GetName() == L"TurretRanged")
		{
			m_pFrameKey = static_cast<CWeapon*>(pBulletWeapon)->GetOwner()->GetTeam() ? L"blueBullet" : L"redBullet";
		}
		else
		{
			m_pFrameKey = L"acher_arrow_r";
		}
	}
	else
	{
		if (pBulletWeapon->GetName() == L"TurretRanged")
		{
			m_pFrameKey = static_cast<CWeapon*>(pBulletWeapon)->GetOwner()->GetTeam() ? L"blueBullet" : L"redBullet";
		}
		else
		{
			m_pFrameKey = L"acher_arrow_l";
		}
	}

	Component_Render(_dc);
	int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	int drawX = int(m_vPos.x * g_fZoom + iScrollX * g_fZoom);
	int drawY = int(m_vPos.y * g_fZoom + iScrollY * g_fZoom);

	if (m_pFrameKey == L"redBullet")
	{
		int spriteW = int(32.f * g_fZoom);
		int spriteH = int(32.f * g_fZoom);

		CBlendingManager::GetInstance()->Render(_dc,
			L"../Image/ApiDemo/Client/fireBall/fireball_r.png",
			Rect(drawX - spriteW / 2, drawY - spriteW / 2, spriteW, spriteW),
			100 * m_tFrame.iFrameStart,
			100 * m_tFrame.iMotion,
			100,
			100,
			1.f);

		return;
	}

	int spriteW = int(m_vScale.x * g_fZoom);
	int spriteH = int(m_vScale.y * g_fZoom);

	HDC   hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);

	if (pBulletWeapon->GetName() == L"TurretRanged")
	{
		GdiTransparentBlt(_dc,
			drawX - (spriteW / 2),
			drawY - (spriteH / 2),
			spriteW,
			spriteH,
			hMemDC,
			0,
			0,
			64,
			64,
			RGB(0, 0, 0));
	}
	else
	{
		GdiTransparentBlt(_dc,
			drawX - (spriteW / 2),
			drawY - (spriteH / 2),
			spriteW,
			spriteH,
			hMemDC,
			0,
			0,
			16,
			16,
			RGB(0, 0, 0));
	}
}

void CBullet::Release()
{
}

void CBullet::OnCollisionEnter(CCollider* _pOther)
{
	if ((GetCollider()->Get_Layer() & _pOther->Get_Mask()) != 0)
	{
		if (static_cast<CWeapon*>(pBulletWeapon)->GetOwner()->GetTeam()
			!= _pOther->GetOwner()->GetTeam())
		{
			static_cast<CRanged*>(pBulletWeapon)->OnHit();
			Set_Dead();
		}
	}
}

void CBullet::OnCollision(CCollider* _pOther)
{
}

void CBullet::CalcAngle()
{
	float m_fWidth = m_pTarget->GetPos().x - m_vPos.x;
	float m_fHeight = m_pTarget->GetPos().y - m_vPos.y;
	float m_fDist = sqrt(m_fWidth * m_fWidth + m_fHeight * m_fHeight);
	m_fAngle = acosf(m_fWidth/m_fDist);
	
	if (m_pTarget->GetPos().y > m_vPos.y)
		m_fAngle *= -1.f;
}

void CBullet::MoveToAngle()
{
	m_vMoveDir.x = cosf(m_fAngle) > 0 ? 1 : -1;
	
	m_vPos.x += (m_fSpeed * fDT) * cosf(m_fAngle);
	m_vPos.y -= (m_fSpeed * fDT) * sinf(m_fAngle);
}
