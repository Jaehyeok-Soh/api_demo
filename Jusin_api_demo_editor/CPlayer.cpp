#include "pch.h"
#include "CPlayer.h"
#include "CKeyManager.h"
#include "CScrollManager.h"
#include "CTimeManager.h"
#include "CBmpManager.h"
#include "CTileManager.h"
#include "CAStarManager.h"
#include "CCollisionManager.h"
#include "CSceneManager.h"
#include "CMelee.h"
#include "CRanged.h"
#include "CSkillSwordman.h"
#include "CUltSwordman.h"
#include "CSkillAcher.h"
#include "CUltAcher.h"
#include "DTOPlayer.h"
#include "CTcpManager.h"
#include "CPeekingManager.h"
#include "CBlendingManager.h"
#include "CGameManager.h"
#include "CEffect.h"
#include "CPngEffect.h"

CPlayer::CPlayer()
	: m_bIsMine(false),
	m_bIsHost(false),
	m_fRenderScale(1.f),
	m_bIsUsingSkill(false),
	m_iCurrentSkill(-1),
	m_strAccount(L""),
	m_fResPawnTime(10.f * 1000.f)
{
	ZeroMemory(&m_tBmpScale, sizeof(BMPSCALE));
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	CreateCollider();

	GetCollider()->SetScale(Vec2(4.f, 4.f));
	GetCollider()->Set_Layer(COL_PLAYER);
	GetCollider()->Set_Mask(COL_MINION
		| COL_TOWER
		| COL_ATTACK
		| COL_PLAYER
		| COL_SKILL
		| COL_WALL);

	m_tStatusInfo.m_iHp = 1000;

	m_vScale = { 16.f, 16.f };
	m_fSpeed = 70.f;
	m_vMoveDir.x = 1.f;

	m_eCurState = IDLE;
	m_ePreState = END;

	switch (m_eJob)
	{
	case CPlayer::SWORDMAN:
		m_fDistance = 10.f;
		m_rgbColor = RGB(255, 255, 255);
		break;
	case CPlayer::ACHER:
		m_fDistance = 50.f;
		m_rgbColor = RGB(0, 0, 0);
		break;
	case CPlayer::MAGICKNIGHT:
		m_fDistance = 15.f;
		break;
	default:
		m_fDistance = 10.f;
		break;
	}

	m_tAttackInfo.m_bIsAttack = false;
	m_tAttackInfo.m_fdtAttackTime = 0.f;
	m_tAttackInfo.m_fAttackDelay = 0.25f;
	m_tAttackInfo.m_iDamage = 10.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 200;

	SetPos(m_vSpawnPos);
	CScrollManager::Get_Instance()->SetScroll(m_vSpawnScroll);

	CreateWeapon();
	CreateSkill();
}

int CPlayer::Update()
{
	if (CSceneManager::GetInstance()->GetCurSceneNum() != SC_PLAY)
		return NOEVENT;

	if (m_tStatusInfo.m_iHp <= 0 && !m_bDead)
	{
		m_eCurState = DIE;
		m_bDead = true;
		m_fDeadTimeCount = GetTickCount();
		SetFrameKey();
		Motion_Change();
	}

	if (m_bDead)
	{
		if (m_eCurState != DIE)
		{
			m_eCurState = DIE;
			m_bDead = true;
			m_fDeadTimeCount = GetTickCount();
			SetFrameKey();
			Motion_Change();
		}

		DeadProc();
		return NOEVENT;
	}

	if (!m_bIsMine && (m_eCurState != ATTACK || m_eCurState != SKILL || m_eCurState != ULT))
	{
		m_bOnTarget = false;
		m_pTarget = nullptr;
		m_iTargetId = -1;
	}

	if (!m_bIsMine && (m_iTargetId > 0))
		FindTargetToId();

	if (m_pTarget && m_pTarget->Get_Dead())
		m_pTarget = nullptr;

	if (m_pCollider)
		m_pCollider->Late_Update();

	Key_Input();

	if (m_eCurState == SKILL || m_eCurState == ULT)
	{
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_eCurState = IDLE;
			m_bIsUsingSkill = false;
		}
	}

	UpdateSkills();

	if (m_eCurState == RUN)
	{
		thread t1(&CPlayer::MoveTile, this);
		t1.join();
	}

	if (m_bOnTarget == true
		&& m_pTarget
		&& m_pTarget->GetCollider() != nullptr
		&& (m_eCurState != SKILL && m_eCurState != ULT))
		AttackProc();

	if (!m_bIsMine
		&& (static_cast<CPlayer*>(CSceneManager::GetInstance()->GetPlayer())->GetHideOption() == m_iHideOption))
	{
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
	}

	SetFrameKey();

	Motion_Change();

	__super::Update_Rect();

	if (m_vMoveDir.x > 0)
		__super::Update_Frame();

	else
		__super::Update_Frame_Reverse();

	if (m_bIsMine && CSceneManager::GetInstance()->GetCurSceneNum() == SC_PLAY)
	{
		ToDTO(false, false, "");
	}

	return NOEVENT;
}

void CPlayer::Late_Update()
{
	if (CSceneManager::GetInstance()->GetCurSceneNum() != SC_PLAY)
		return;

	if (m_pCollider)
		m_pCollider->Late_Update();
}

void CPlayer::Render(HDC _dc)
{
	if (CSceneManager::GetInstance()->GetCurSceneNum() != SC_PLAY)
		return;

	Component_Render(_dc);

	if (!m_bIsMine 
		&& m_bIsHide 
		&& (m_bTeam != static_cast<CPlayer*>(CSceneManager::GetInstance()->GetPlayer())->GetTeam()))
	{
		if (static_cast<CPlayer*>(CSceneManager::GetInstance()->GetPlayer())->GetHideOption() != m_iHideOption)
			return;
	}

	int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	int drawX = int(m_vPos.x * g_fZoom + iScrollX * g_fZoom);
	int drawY = int(m_vPos.y * g_fZoom + iScrollY * g_fZoom);

	int spriteW = int(m_vScale.x * g_fZoom);
	int spriteH = int(m_vScale.y * g_fZoom);

	HDC   hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_dc,
		drawX - ((spriteW / 2) * m_fRenderScale),
		drawY - ((spriteH / 2) * m_fRenderScale),
		spriteW * m_fRenderScale,
		spriteH * m_fRenderScale,
		hMemDC,
		(int)m_tBmpScale.iWidth * m_tFrame.iFrameStart,
		(int)m_tBmpScale.iHeight * m_tFrame.iMotion,
		(int)m_tBmpScale.iWidth,
		(int)m_tBmpScale.iHeight,
		m_rgbColor);

	int barSpriteW = int(8.25f * g_fZoom);
	int barSpriteH = int(1.375f * g_fZoom);

	auto a = barSpriteW * (m_tStatusInfo.m_iHp / 1000);

	CBlendingManager::GetInstance()->RenderBlend(_dc,
		L"../Image/UI/Gauge/volume_guage.png",
		Rect(drawX - barSpriteW / 2, drawY - barSpriteH / 2 - 20, (int)(barSpriteW * ((float)m_tStatusInfo.m_iHp / 1000.f)), barSpriteH),
		0, 0,
		73, 5,
		1.f);

	CBlendingManager::GetInstance()->RenderBlend(_dc,
		L"../Image/UI/Gauge/guage_bg.png",
		Rect(drawX - barSpriteW / 2, drawY - barSpriteH / 2 - 20, barSpriteW, barSpriteH),
		0, 0,
		46, 11,
		1.f);

	if (m_strAccount != L"")
	{
		TextOut(_dc,
			drawX - barSpriteW / 2,
			drawY - barSpriteH / 2 - 40,
			m_strAccount.c_str(),
			lstrlen(m_strAccount.c_str()));
	}

	if (!bColRender)
		return;

	DebugTextOut(_dc);
}

void CPlayer::Release()
{
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
}

void CPlayer::OnCollision(CCollider* _pOther)
{
	CCollisionManager::Collision_Rect_Resolve(GetCollider(), _pOther);
}

void CPlayer::OnPeek(CObject* _pTargetObj)
{
	m_pTarget = _pTargetObj;
	m_bOnTarget = true;
	ChaseTarget();
	m_eCurState = RUN;
}

void CPlayer::Key_Input()
{
	if (m_bIsMine == false || m_bDead)
		return;

	POINT vWorldMouse;
	vWorldMouse.x = g_ptMousePos.x / g_fZoom - (int)CScrollManager::Get_Instance()->Get_ScrollX();
	vWorldMouse.y = g_ptMousePos.y / g_fZoom - (int)CScrollManager::Get_Instance()->Get_ScrollY();

	if (CKeyManager::Get_Instance()->Key_Down(VK_RBUTTON))
	{
		if (g_ptMousePos.x > WINCX - 356.f
			&& g_ptMousePos.y > WINCY - 200.f
			&& g_ptMousePos.x < WINCX
			&& g_ptMousePos.y < WINCY)
			return;

		if (CTileManager::Get_Instance()->Peeking_Tile(vWorldMouse))
		{
			Vec2 vTilePos = CTileManager::Get_Instance()->MouseToTile(vWorldMouse);
			vTilePos = Vec2((int)vTilePos.x / TILECX, (int)vTilePos.y / TILECY);
			Vec2 startIdx = Vec2((int)(m_vPos.x / TILECX), (int)(m_vPos.y / TILECY));
			Vec2 endIdx = vTilePos;

			auto future = async(launch::async,
				&CAStarManager::FindPath,
				CAStarManager::GetInstance(),
				startIdx, endIdx);

			m_Path = future.get();

			if (!m_Path.empty())
			{
				m_Path.pop_front();
				if (m_eCurState == ATTACK)
					m_tFrame.iFrameStart = 0;
				AttackInit();
				m_eCurState = RUN;
				m_bOnTarget = false;
				m_pTarget = nullptr;
			}
		}
	}

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
	}

	if (CKeyManager::Get_Instance()->Key_Pressing('Q'))
	{
		if (m_vSkills.size() > 0)
		{
			m_vSkills[0]->Use(*this, m_vMoveDir.x);
			m_bIsUsingSkill = true;
			m_iCurrentSkill = 0;
		}
	}

	if (CKeyManager::Get_Instance()->Key_Pressing('W'))
	{
		if (m_vSkills.size() > 1)
		{
			m_vSkills[1]->Use(*this, m_vMoveDir.x);
			m_bIsUsingSkill = true;
			m_iCurrentSkill = 1;
		}
	}
}

void CPlayer::Offset()
{
	int iCenterX = WINCX >> 1;
	int   iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int   iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	float fPlayerScreenX = m_vPos.x + iScrollX;
	float fPlayerScreenY = m_vPos.y + iScrollY;

	const float DEADZONE_X = 10.f; // X축 데드존
	const float DEADZONE_Y = 20.f; // Y축 데드존

	if (CScrollManager::Get_Instance()->Get_ScrollXMax() >= m_vPos.x + iScrollX)
	{
		// X축 중심 고정
		if (fPlayerScreenX < iCenterX - DEADZONE_X)
			CScrollManager::Get_Instance()->Set_ScrollX(m_fSpeed * fDT);

		else if (fPlayerScreenX > iCenterX + DEADZONE_X)
			CScrollManager::Get_Instance()->Set_ScrollX(-m_fSpeed * fDT);
	}

	int   iOffsetminY = 300;
	int   iOffsetmaxY = 400;

	if (CScrollManager::Get_Instance()->Get_ScrollYMax() >= m_vPos.y + iScrollY)
	{
		if (iOffsetminY > m_vPos.y + iScrollY)
			CScrollManager::Get_Instance()->Set_ScrollY(m_fSpeed * fDT);

		if (iOffsetmaxY < m_vPos.y + iScrollY)
			CScrollManager::Get_Instance()->Set_ScrollY(-m_fSpeed * fDT);
	}
}

void CPlayer::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		if (m_eJob == SWORDMAN)
		{
			switch (m_eCurState)
			{
			case IDLE:
				m_tFrame.iFrameStart = (m_vMoveDir.x > 0) ? 0 : 4;
				m_tFrame.iFrameEnd = (m_vMoveDir.x > 0) ? 4 : 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 200;

				m_tBmpScale.iWidth = 64;
				m_tBmpScale.iHeight = 64;

				m_fRenderScale = 1.f;
				break;
			case RUN:
				m_tFrame.iFrameStart = (m_vMoveDir.x > 0) ? 0 : 7;
				m_tFrame.iFrameEnd = (m_vMoveDir.x > 0) ? 7 : 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 200;

				m_tBmpScale.iWidth = 64;
				m_tBmpScale.iHeight = 64;

				m_fRenderScale = 1.f;
				break;
			case ATTACK:
				m_tFrame.iFrameStart = (m_vMoveDir.x > 0) ? 0 : 4;
				m_tFrame.iFrameEnd = (m_vMoveDir.x > 0) ? 4 : 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 200;

				m_tBmpScale.iWidth = 64;
				m_tBmpScale.iHeight = 64;

				m_fRenderScale = 1.f;
				break;
			case SKILL:
				m_tFrame.iFrameStart = (m_vMoveDir.x > 0) ? 0 : 8;
				m_tFrame.iFrameEnd = (m_vMoveDir.x > 0) ? 8 : 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 100;

				m_tBmpScale.iWidth = 128;
				m_tBmpScale.iHeight = 128;

				m_fRenderScale = 2.f;
				break;
			case ULT:
				m_tFrame.iFrameStart = (m_vMoveDir.x > 0) ? 0 : 7;
				m_tFrame.iFrameEnd = (m_vMoveDir.x > 0) ? 7 : 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 10;

				m_tBmpScale.iWidth = 64;
				m_tBmpScale.iHeight = 64;

				m_fRenderScale = 1.f;
				break;
			case DIE:
				m_tFrame.iFrameStart = (m_vMoveDir.x > 0) ? 0 : 7;
				m_tFrame.iFrameEnd = (m_vMoveDir.x > 0) ? 7 : 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 200;

				m_tBmpScale.iWidth = 64;
				m_tBmpScale.iHeight = 64;

				m_fRenderScale = 1.f;
				break;
			}
		}
		else if (m_eJob == ACHER)
		{
			switch (m_eCurState)
			{
			case IDLE:
				m_tFrame.iFrameStart = (m_vMoveDir.x > 0) ? 0 : 4;
				m_tFrame.iFrameEnd = (m_vMoveDir.x > 0) ? 4 : 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 200;

				m_tBmpScale.iWidth = 64;
				m_tBmpScale.iHeight = 64;

				m_fRenderScale = 1.f;
				break;
			case RUN:
				m_tFrame.iFrameStart = (m_vMoveDir.x > 0) ? 0 : 8;
				m_tFrame.iFrameEnd = (m_vMoveDir.x > 0) ? 8 : 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 200;

				m_tBmpScale.iWidth = 64;
				m_tBmpScale.iHeight = 64;

				m_fRenderScale = 1.f;
				break;
			case ATTACK:
				m_tFrame.iFrameStart = (m_vMoveDir.x > 0) ? 0 : 5;
				m_tFrame.iFrameEnd = (m_vMoveDir.x > 0) ? 5 : 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 200;

				m_tBmpScale.iWidth = 64;
				m_tBmpScale.iHeight = 64;

				m_fRenderScale = 1.f;
				break;
			case SKILL:
				m_tFrame.iFrameStart = (m_vMoveDir.x > 0) ? 0 : 7;
				m_tFrame.iFrameEnd = (m_vMoveDir.x > 0) ? 7 : 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 100;

				m_tBmpScale.iWidth = 64;
				m_tBmpScale.iHeight = 64;

				m_fRenderScale = 1.f;
				break;
			case ULT:
				m_tFrame.iFrameStart = (m_vMoveDir.x > 0) ? 0 : 8;
				m_tFrame.iFrameEnd = (m_vMoveDir.x > 0) ? 8 : 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 200;

				m_tBmpScale.iWidth = 64;
				m_tBmpScale.iHeight = 64;

				m_fRenderScale = 1.f;
				break;
			case DIE:
				m_tFrame.iFrameStart = (m_vMoveDir.x > 0) ? 0 : 9;
				m_tFrame.iFrameEnd = (m_vMoveDir.x > 0) ? 9 : 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwTime = GetTickCount();
				m_tFrame.dwSpeed = 200;

				m_tBmpScale.iWidth = 64;
				m_tBmpScale.iHeight = 64;

				m_fRenderScale = 1.f;
				break;
			}
		}

		m_tFrame.iStartBuffer = m_tFrame.iFrameStart;

		m_ePreState = m_eCurState;
	}
}

LPCWSTR CPlayer::GetStateName(STATE eState)
{
	switch (eState)
	{
	case CCharacter::IDLE:
		return L"IDLE";
	case CCharacter::RUN:
		return L"RUN";
	case CCharacter::ATTACK:
		return L"ATTACK";
	case CCharacter::SKILL:
		return L"SKILL";
	case CCharacter::ULT:
		return L"ULT";
	case CCharacter::DIE:
		return L"DIE";
	default:
		return L"UNKNOWN";
	}
	return LPCWSTR();
}

void CPlayer::MakeFrameKey(const TCHAR* strJob, const TCHAR* strDir)
{
	const TCHAR* keyword = nullptr;
	switch (m_eCurState)
	{
	case CCharacter::IDLE:
		keyword = L"_idle_";
		break;
	case CCharacter::RUN:
		keyword = L"_run_";
		break;
	case CCharacter::ATTACK:
		keyword = L"_attack_";
		break;
	case CCharacter::SKILL:
		keyword = L"_skill_";
		break;
	case CCharacter::ULT:
		keyword = L"_ult_";
		break;
	case CCharacter::DIE:
		keyword = L"_die_";
		break;
	case CCharacter::END:
	default:
		keyword = L"_idle_";
		break;
	}
	m_strFrameKey = strJob;
	m_strFrameKey += keyword;
	m_strFrameKey += strDir;

	m_pFrameKey = m_strFrameKey.c_str();
}

wstring CPlayer::SetFrameKey()
{
	if (m_vMoveDir.x > 0)
	{
		switch (m_eJob)
		{
		case CPlayer::SWORDMAN:
			MakeFrameKey(L"swordman", L"r");
			break;
		case CPlayer::ACHER:
			MakeFrameKey(L"acher", L"r");
			break;
		case CPlayer::MAGICKNIGHT:
			MakeFrameKey(L"magicknight", L"r");
			break;
		}
	}
	else
	{
		switch (m_eJob)
		{
		case CPlayer::SWORDMAN:
			MakeFrameKey(L"swordman", L"l");
			break;
		case CPlayer::ACHER:
			MakeFrameKey(L"acher", L"l");
			break;
		case CPlayer::MAGICKNIGHT:
			MakeFrameKey(L"magicknight", L"l");
			break;
		}
	}

	return wstring();
}

void CPlayer::MoveTile()
{
	if (m_eCurState == RUN && !m_Path.empty())
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
			m_vMoveDir.x = toNext.Normalize().x;
			m_vPos += toNext * moveStep;
		}

		if (m_Path.empty())
			m_eCurState = IDLE;
	}
}

void CPlayer::DebugTextOut(HDC _dc)
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
#pragma region 현재좌표x
	std::wstring posX = std::to_wstring(m_vPos.x);
	LPCWSTR szPosX = posX.c_str();
	TextOut(_dc,
		(int)drawX - 30,
		(int)drawY - 90,
		szPosX,
		lstrlen(szPosX));
#pragma endregion
#pragma region 현재좌표Y
	std::wstring posY = std::to_wstring(m_vPos.y);
	LPCWSTR szPosY = posY.c_str();
	TextOut(_dc,
		(int)drawX + 30,
		(int)drawY - 90,
		szPosY,
		lstrlen(szPosY));
#pragma endregion
#pragma region 랜더좌표x
	std::wstring RposX = std::to_wstring(drawX - spriteW / 2);
	LPCWSTR szRPosX = RposX.c_str();
	TextOut(_dc,
		(int)drawX - 30,
		(int)drawY - 110,
		szRPosX,
		lstrlen(szRPosX));
#pragma endregion
#pragma region 렌더좌표Y
	std::wstring RposY = std::to_wstring(drawY - spriteH / 2);
	LPCWSTR szRPosY = RposY.c_str();
	TextOut(_dc,
		(int)drawX + 30,
		(int)drawY - 110,
		szRPosY,
		lstrlen(szRPosY));
#pragma endregion
}

void CPlayer::CreateWeapon()
{
	switch (m_eJob)
	{
	case CPlayer::SWORDMAN:
	case CPlayer::MAGICKNIGHT:
	{
		if (!m_pWeapon)
		{
			m_pWeapon = new CMelee();
			m_pWeapon->SetName(L"Melee");
		}
	}
	break;
	case CPlayer::ACHER:
	{
		if (!m_pWeapon)
		{
			m_pWeapon = new CRanged();
			m_pWeapon->SetName(L"Ranged");
		}
	}
	break;
	}

	m_pWeapon->Initialize(this, m_tAttackInfo);
	CSceneManager::GetInstance()->GetCurScene()->AddObject(m_pWeapon, OBJ_WEAPON);
	
	if (m_bIsMine)
		CSceneManager::GetInstance()->SetWeapon(m_pWeapon);
	else
		CSceneManager::GetInstance()->AddOtherWeapon(m_pWeapon);
}

void CPlayer::CreateSkill()
{
	switch (m_eJob)
	{
	case CPlayer::SWORDMAN:
		m_vSkills.push_back(new CSkillSwordman());
		m_vSkills.push_back(new CUltSwordman());
		break;
	case CPlayer::ACHER:
		m_vSkills.push_back(new CSkillAcher());
		m_vSkills.push_back(new CUltAcher());
		break;
	case CPlayer::MAGICKNIGHT:
		break;
	}

}

void CPlayer::AttackProc()
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
			m_Path.clear();
			m_eCurState = ATTACK;
		}
		else
		{
			ChaseTarget();
			m_eCurState = RUN;
		}

		if (m_pTarget->Get_Dead())
		{
			FindTarget();
			if (m_Path.empty())
			{
				m_eCurState = IDLE;
				m_bOnTarget = false;
				m_tAttackInfo.m_bIsAttack = false;
				m_tAttackInfo.m_fdtAttackTime = 0.f;
			}
			else
				m_eCurState = RUN;
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

void CPlayer::AttackInit()
{
	m_tAttackInfo.m_fdtAttackTime = 0.f;
	m_tAttackInfo.m_bIsAttack = false;
}

void CPlayer::UpdateSkills()
{
	for (auto skill : m_vSkills)
	{
		skill->Update(*this);
	}
}

void CPlayer::ToDTO(bool isStart, bool isQuit, string winner)
{
	size_t bufferSize = (wcslen((wchar_t*)m_pFrameKey) + 1) * 4;
	char* buffer = new char[bufferSize];
	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, buffer, bufferSize, m_pFrameKey, _TRUNCATE);
	string strSendFrameKey = buffer;

	auto str = CGameManager::GetInstance()->GetAccount();
	bufferSize = (wcslen((wchar_t*)str.c_str()) + 1) * 4;
	buffer = new char[bufferSize];
	convertedChars = 0;
	wcstombs_s(&convertedChars, buffer, bufferSize, str.c_str(), _TRUNCATE);
	string strSendAccount = buffer;

	auto tDtoPlayer = DTOPLAYER{
		m_iNetId,
		m_iObjectId,
		m_pTarget ? m_pTarget->GetObjectId() : -1,
		m_tStatusInfo.m_iHp,
		m_strSendName,
		m_vPos.x,
		m_vPos.y,
		(m_vMoveDir.x > 0) ? 1 : -1,
		strSendFrameKey,
		m_tFrame.iFrameStart,
		m_eCurState,
		-1, //none
		m_bIsUsingSkill,
		m_iCurrentSkill,
		isStart,
		isQuit,
		winner,
		strSendAccount
	};

	json j = tDtoPlayer;
	string msg = j.dump() + "\n";
	CTcpManager::GetInstance()->SendSocket(msg);

	delete[] buffer;
}

void CPlayer::fromJson()
{
}

void CPlayer::DeadProc()
{
	if (m_fDeadTimeCount + m_fResPawnTime < GetTickCount())
	{
		m_tStatusInfo.m_iHp = 1000;
		m_bDead = false;

		SetPos(m_vSpawnPos);

		CScrollManager::Get_Instance()->SetScroll(m_vSpawnScroll);

		m_eCurState = IDLE;

		CPngEffect* pEffect = new CPngEffect();
		FRAME tFrame;
		tFrame.iFrameStart = 0;
		tFrame.iFrameEnd = 19;
		tFrame.iMotion = 0;
		tFrame.iStartBuffer = 0;
		tFrame.dwSpeed = 100;
		tFrame.dwTime = GetTickCount();
		BMPSCALE tScale;
		tScale.iWidth = 70;
		tScale.iHeight = 55;
		pEffect->SetPos(m_vPos);
		pEffect->SetScale(Vec2(70.f, 55.f));
		pEffect->Initialize(tFrame, tScale, L"swordman_skill_ef_r", RGB(255, 255, 255));
		CSceneManager::GetInstance()->GetCurScene()->AddObject(pEffect, OBJ_EFFECT);
		return;
	}

	if (m_tFrame.iFrameStart != m_tFrame.iFrameEnd)
		__super::Update_Frame();
}