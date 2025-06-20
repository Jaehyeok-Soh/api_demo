#include "pch.h"
#include "CMinion.h"
#include "CScrollManager.h"
#include "CTimeManager.h"
#include "CBmpManager.h"
#include "CTileManager.h"
#include "CAStarManager.h"
#include "CCollisionManager.h"
#include "CKeyManager.h"
#include "CPeekingManager.h"
#include "CSceneManager.h"
#include "CMelee.h"

CMinion::CMinion()
{
}

CMinion::~CMinion()
{
	Release();
}

void CMinion::Initialize()
{
	CreateCollider();

	//CreateGravity();
	GetCollider()->SetScale(Vec2(4.f, 4.f));
	GetCollider()->Set_Layer(COL_MINION);
	GetCollider()->Set_Mask(COL_MINION
		| COL_TOWER
		| COL_ATTACK
		| COL_PLAYER
		| COL_SKILL);

	m_tStatusInfo.m_iHp = 100;

	m_vScale = { 16.f, 16.f };
	m_fSpeed = 100.f;
	m_fDistance = 20.f;

	m_eCurState = MOVE;
	m_ePreState = END;

	m_bOnTarget = false;

	m_tAttackInfo.m_bIsAttack = false;
	m_tAttackInfo.m_fdtAttackTime = 0.f;
	m_tAttackInfo.m_fAttackDelay = 0.25f;
	m_tAttackInfo.m_iDamage = 10.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 200;

	if (m_bTeam)
	{
		m_vTatgetNexusTile = Vec2(96.f, 13.f);
		m_vMoveDir.x = 1.f;
	}
	else
	{
		m_vTatgetNexusTile = Vec2(13.f, 44.f);
		m_vMoveDir.x = -1.f;
	}

	ChaseNexus(true);

	CreateWeapon();
}

int CMinion::Update()
{
	if (m_tStatusInfo.m_iHp <= 0)
		Set_Dead();

	if (m_bDead)
		return DEAD;

	POINT ptMouse;
	GetCursorPos(&ptMouse); // 화면 좌표
	ScreenToClient(g_hWnd, &ptMouse); // 클라이언트 좌표로 변환

	POINT vWorldMouse;
	vWorldMouse.x = ptMouse.x / g_fZoom - CScrollManager::Get_Instance()->Get_ScrollX();
	vWorldMouse.y = ptMouse.y / g_fZoom - CScrollManager::Get_Instance()->Get_ScrollY();

	if (PtInRect(&m_tRect, vWorldMouse))
	{
		if (CKeyManager::Get_Instance()->Key_Pressing(VK_RBUTTON))
		{
			CPeekingManager::GetInstance()->OnPeek(this);
		}
	}

	if (!m_bOnTarget || !m_pTarget || m_pTarget->Get_Dead())
		FindTarget();

	if (m_bOnTarget || m_eCurState == ATTACK)
		AttackPoc();

	if (!m_bOnTarget && m_eCurState == ATTACK)
		ChaseNexus(false);

	if (m_bOnTarget && m_pTarget->Get_Dead())
	{
		m_pTarget = nullptr;
		m_bOnTarget = false;
	}

	if (m_eCurState == MOVE)
	{
		thread t1(&CMinion::MoveTile, this);
		t1.join();
	}

	SetFrameKey();

	Motion_Change();

	__super::Update_Rect();
	__super::Update_Frame();

	return NOEVENT;
}

void CMinion::Late_Update()
{
	if (m_pCollider)
		m_pCollider->Late_Update();
}

void CMinion::Render(HDC _dc)
{
	Component_Render(_dc);
	int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	int drawX = int(m_vPos.x * g_fZoom + iScrollX * g_fZoom);
	int drawY = int(m_vPos.y * g_fZoom + iScrollY * g_fZoom);

	int spriteW = int(m_vScale.x * g_fZoom);
	int spriteH = int(m_vScale.y * g_fZoom);

	HDC   hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_dc,
		drawX - spriteW / 2,
		drawY - spriteH / 2,
		spriteW,
		spriteH,
		hMemDC,
		(int)46 * m_tFrame.iFrameStart,
		(int)41 * m_tFrame.iMotion,
		(int)46,   // 복사할 비트맵 가로 세로 사이즈
		(int)41,
		RGB(255, 0, 255));   // 제거할 픽셀 색상 값

	DebugTextOut(_dc);
}

void CMinion::Release()
{
}

void CMinion::OnCollisionEnter(CCollider* _pOther)
{
}

void CMinion::OnCollision(CCollider* _pOther)
{
	CCollisionManager::Collision_Rect_Resolve(GetCollider(), _pOther);
}


void CMinion::CreateWeapon()
{
	if (!m_pWeapon)
	{
		m_pWeapon = new CMelee();
		m_pWeapon->SetName(L"Melee");
	}

	m_pWeapon->Initialize(this, m_tAttackInfo);
	CSceneManager::GetInstance()->GetCurScene()->AddObject(m_pWeapon, OBJ_WEAPON);
}

void CMinion::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;
		case MOVE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;
		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;
		}

		m_ePreState = m_eCurState;
	}
}

LPCWSTR CMinion::GetStateName(MINIONSTATE eState)
{
	switch (eState)
	{
	case MOVE:
		return L"MOVE";
	case ATTACK:
		return L"ATTACK";
	default:
		return L"UNKNOWN";
	}
}

void CMinion::SetFrameKey()
{
	if (m_bTeam)
		m_strFrameKey = L"Blue";
	else
		m_strFrameKey = L"Red";

	if (m_vMoveDir.x > 0)
	{
		m_strFrameKey += L"Minion_r";
	}
	else
	{
		m_strFrameKey += L"Minion_l";
	}

	m_pFrameKey = m_strFrameKey.c_str();
}

void CMinion::MoveTile()
{
	if (m_eCurState == MOVE && !m_Path.empty())
	{
		Vec2 next = m_Path.front();
		Vec2 toNext = next - m_vPos;
		float dist = toNext.Length();
		float moveStep = m_fSpeed * fDT;

		if (dist <= moveStep)
		{
			m_vPos = next;
			m_Path.pop_front();
		}
		else
		{
			toNext.Normalize();
			m_vPos += toNext * moveStep;
		}

		if (m_Path.empty())
			m_eCurState = MOVE;
	}
}

void CMinion::DebugTextOut(HDC _dc)
{
	int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	int drawX = int(m_vPos.x * g_fZoom + iScrollX * g_fZoom);
	int drawY = int(m_vPos.y * g_fZoom + iScrollY * g_fZoom);

	int spriteW = int(m_vScale.x * g_fZoom);
	int spriteH = int(m_vScale.y * g_fZoom);

#pragma region 테스트용
	LPCWSTR szState = GetStateName(m_eCurState);
	TextOut(_dc,
		(int)drawX - 30,
		(int)drawY - 70,
		szState,
		lstrlen(szState));
#pragma endregion
#pragma region 테스트용
	std::wstring wstrHP = std::to_wstring(m_tStatusInfo.m_iHp);
	LPCWSTR szHP = wstrHP.c_str();
	TextOut(_dc,
		(int)drawX + 60,
		(int)drawY - 70,
		szHP,
		lstrlen(szHP));
#pragma endregion
}

void CMinion::AttackPoc()
{
	if (!m_tAttackInfo.m_bIsAttack)
	{
		if (Get_DistToTarget() <= m_fDistance + (m_pTarget->GetScale().x * 0.5f))
		{
			m_Path.clear();
			m_eCurState = ATTACK;
		}
		else
		{
			ChaseTarget();
			m_eCurState = MOVE;
		}

		if (Get_DistToTarget() >= 100.f)
		{
			ChaseNexus(false);
		}

		if ((m_pTarget && m_pTarget->Get_Dead())
			|| !m_pTarget)
		{
			ChaseNexus(false);
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

		if ((m_pTarget && m_pTarget->Get_Dead())
			|| !m_pTarget)
		{
			ChaseNexus(false);
		}
	}
}

void CMinion::AttackInit()
{
	m_tAttackInfo.m_fdtAttackTime = 0.f;
	m_tAttackInfo.m_bIsAttack = false;
	if (m_pTarget)
	{
		ChaseTarget();
		m_eCurState = MOVE;
	}
	else
	{
		ChaseNexus(false);
	}
}

void CMinion::ChaseNexus(bool _bIsInit)
{
	m_Path.clear();

	Vec2 startIdx;
	if (m_bTeam)
	{
		if (_bIsInit)
			startIdx = Vec2(13.f, 44.f);
		else
			startIdx = Vec2(m_vPos.x / TILECX, m_vPos.y / TILECY);
	}
	else
	{
		if (_bIsInit)
			startIdx = Vec2(96.f, 13.f);
		else
			startIdx = Vec2(m_vPos.x / TILECX, m_vPos.y / TILECY);
	}

	auto future = async(launch::async,
		&CAStarManager::FindPath,
		CAStarManager::GetInstance(),
		startIdx, m_vTatgetNexusTile);

	m_Path = future.get();

	if (!m_Path.empty())
	{
		m_Path.pop_front();
		m_eCurState = MOVE;
		m_bOnTarget = false;
		m_pTarget = nullptr;
		m_tAttackInfo.m_bIsAttack = false;
		m_tAttackInfo.m_fdtAttackTime = 0.f;
	}
}

