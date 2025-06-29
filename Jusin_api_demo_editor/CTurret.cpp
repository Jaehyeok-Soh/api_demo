#include "pch.h"
#include "CTurret.h"
#include "CScrollManager.h"
#include "CBmpManager.h"
#include "CKeyManager.h"
#include "CPeekingManager.h"
#include "CRanged.h"
#include "CSceneManager.h"
#include "CTimeManager.h"
#include "CGameManager.h"
#include "CBlendingManager.h"

CTurret::CTurret()
	:strFrameBlueTurret(L"BlueTurretSmall"), strFrameRedTurret(L"RedTurretSmall")
{
}

CTurret::~CTurret()
{
}

void CTurret::Initialize()
{
	__super::Initialize();

	CreateCollider();

	GetCollider()->SetScale(Vec2(8.f, 8.f));
	GetCollider()->Set_Layer(COL_TOWER);
	GetCollider()->Set_Mask(COL_MINION
		| COL_PLAYER
		| COL_ATTACK);

	m_tStatusInfo.m_iHp = 100;

	m_fDistance = 50.f;

	m_tAttackInfo.m_bIsAttack = false;
	m_tAttackInfo.m_fdtAttackTime = 0.f;
	m_tAttackInfo.m_fAttackDelay = 2.f;
	m_tAttackInfo.m_iDamage = 20;

	CreateWeapon();
}

int CTurret::Update()
{
	if (m_bDead)
	{
		CGameManager::GetInstance()->SetSequence(m_eDrawID, m_iOption);
		return DEAD;
	}

	POINT ptMouse;
	GetCursorPos(&ptMouse); // 화면 좌표
	ScreenToClient(g_hWnd, &ptMouse); // 클라이언트 좌표로 변환

	POINT vWorldMouse;
	vWorldMouse.x = ptMouse.x / g_fZoom - CScrollManager::Get_Instance()->Get_ScrollX();
	vWorldMouse.y = ptMouse.y / g_fZoom - CScrollManager::Get_Instance()->Get_ScrollY();

	if (PtInRect(&m_tRect, vWorldMouse) && CheckAttackable())
	{
		if (CKeyManager::Get_Instance()->Key_Pressing(VK_RBUTTON))
		{
			CPeekingManager::GetInstance()->OnPeek(this);
		}
	}

	if (!m_bOnTarget || !m_pTarget || m_pTarget->Get_Dead())
		FindTarget();

	if (m_bOnTarget || m_eCurState == ATTACK)
		AttackProc();

	if (m_bOnTarget && m_pTarget->Get_Dead())
	{
		m_pTarget = nullptr;
		m_bOnTarget = false;
	}

	//__super::Update();
	__super::Update_Rect();
	return NOEVENT;
}

void CTurret::Late_Update()
{
	if (m_pCollider)
		m_pCollider->Late_Update();
}

void CTurret::Render(HDC _dc)
{
	Component_Render(_dc);
	int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	int drawX = int(m_vPos.x * g_fZoom + iScrollX * g_fZoom);
	int drawY = int(m_vPos.y * g_fZoom + iScrollY * g_fZoom);

	int spriteW = int(m_vScale.x * g_fZoom);
	int spriteH = int(m_vScale.y * g_fZoom);

	if (m_bTeam)
	{
		m_pFrameKey = strFrameBlueTurret;
		HDC hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);

		auto a = GdiTransparentBlt(_dc,
			drawX - spriteW / 2,
			drawY - spriteH / 2 - 50,
			spriteW,
			spriteH + 50,
			hMemDC,
			0,
			0,
			87,   // 복사할 비트맵 가로 세로 사이즈
			149,
			RGB(189, 189, 189));   // 제거할 픽셀 색상 값
		auto b = 1;
	}
	else
	{
		m_pFrameKey = strFrameRedTurret;
		HDC hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);
		GdiTransparentBlt(_dc,
			drawX - spriteW / 2,
			drawY - spriteH / 2 - 50,
			spriteW,
			spriteH + 50,
			hMemDC,
			0,
			0,
			94,   // 복사할 비트맵 가로 세로 사이즈
			169,
			RGB(189, 189, 189));   // 제거할 픽셀 색상 값
	}

	int barSpriteW = int(8.25f * g_fZoom);
	int barSpriteH = int(1.375f * g_fZoom);

	CBlendingManager::GetInstance()->RenderBlend(_dc,
		L"../Image/UI/Gauge/volume_guage.png",
		Rect(drawX - barSpriteW / 2, drawY - barSpriteH / 2 - 20, (int)(barSpriteW * ((float)m_tStatusInfo.m_iHp / 100.f)), barSpriteH),
		0, 0,
		73, 5,
		1.f);

	CBlendingManager::GetInstance()->RenderBlend(_dc,
		L"../Image/UI/Gauge/guage_bg.png",
		Rect(drawX - barSpriteW / 2, drawY - barSpriteH / 2 - 20, barSpriteW, barSpriteH),
		0, 0,
		46, 11,
		1.f);
}

void CTurret::Release()
{
}

void CTurret::CreateWeapon()
{
	if (!m_pWeapon)
	{
		m_pWeapon = new CRanged();
		m_pWeapon->SetName(L"TurretRanged");
		m_pWeapon->Initialize(this, m_tAttackInfo);
		CSceneManager::GetInstance()->GetCurScene()->AddObject(m_pWeapon, OBJ_WEAPON);
	}
}

void CTurret::AttackProc()
{
	if (m_pTarget == nullptr)
	{
		m_bOnTarget = false;
		m_eCurState = IDLE;
		AttackInit();
		return;
	}

	if (!m_tAttackInfo.m_bIsAttack)
	{
		if (Get_DistToTarget() <= m_fDistance + (m_pTarget->GetScale().x * 0.5f))
		{
			//공격상태전환
			m_eCurState = ATTACK;
		}
		else
		{
			//대기
			//타겟찾기
			m_eCurState = IDLE;
			FindTarget();
		}

		if (m_pTarget->Get_Dead())
		{
			FindTarget();
			if (m_pTarget == nullptr)
			{
				m_eCurState = IDLE;
				m_bOnTarget = false;
				AttackInit();
			}
			else
				m_eCurState = IDLE;
		}

		if (m_eCurState == ATTACK)
		{
			m_pWeapon->Attack();
			m_tAttackInfo.m_bIsAttack = true;
		}
	}
	else
	{
		m_tAttackInfo.m_fdtAttackTime += fDT;
		if (m_tAttackInfo.m_fdtAttackTime >= m_tAttackInfo.m_fAttackDelay)
		{
			AttackInit();
		}
	}
}

void CTurret::AttackInit()
{
	m_tAttackInfo.m_fdtAttackTime = 0.f;
	m_tAttackInfo.m_bIsAttack = false;
}
