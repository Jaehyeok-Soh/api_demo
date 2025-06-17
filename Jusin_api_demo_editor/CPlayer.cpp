#include "pch.h"
#include "CPlayer.h"
#include "CKeyManager.h"
#include "CScrollManager.h"
#include "CTimeManager.h"
#include "CBmpManager.h"
#include "CTileManager.h"
#include "CAStarManager.h"
#include "CCollisionManager.h"

CPlayer::CPlayer()
	: m_bIsMine(false),
	m_bIsHost(false),
	m_fPlayTime(0.f)
{
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	CreateCollider();

	//CreateGravity();
	GetCollider()->SetOffsetPos(Vec2(0.f, 65.f));
	GetCollider()->SetScale(Vec2(8.f, 8.f));
	GetCollider()->Set_Layer(COL_PLAYER);
	GetCollider()->Set_Mask(COL_MINION
		| COL_TOWER
		| COL_ATTACK
		| COL_PLAYER
		| COL_SKILL);

	CScrollManager::Get_Instance()->Set_ScrollX(-10.f);
	CScrollManager::Get_Instance()->Set_ScrollY(-750.f);

	m_iHP = 100;

	m_vPos = Vec2{ 50, 800 };
	m_vScale = { 32.f, 32.f };
	m_fSpeed = 300.f;
	m_vMoveDir.x = 1.f;

	m_pFrameKey = L"swordman_idle_r";

	m_eCurState = IDLE;
	m_ePreState = END;
	m_eJob = SWORDMAN;

	switch (m_eJob)
	{
	case CPlayer::SWORDMAN:
		m_fDistance = 100.f;
		break;
	case CPlayer::ACHER:
		m_fDistance = 500.f;
		break;
	case CPlayer::MAGICKNIGHT:
		m_fDistance = 150.f;
		break;
	default:
		m_fDistance = 100.f;
		break;
	}

	m_tAttackInfo.m_bIsAttack = false;
	m_tAttackInfo.m_fdtAttackTime = 0.f;
	m_tAttackInfo.m_fAttackDelay = 5.f;
	m_tAttackInfo.m_iDamage = 10.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 200;
}

int CPlayer::Update()
{
 	if (m_pCollider)
		m_pCollider->Late_Update();

	Key_Input();
	
	if (m_eCurState == RUN)
	{
		/*thread t1(&CPlayer::MoveVector, this);
		t1.join();*/
		MoveVector();
	}
	
	if (m_bOnTarget == true)
		AttackPoc();

	SetFrameKey();

	Motion_Change();

	__super::Update_Rect();
	__super::Update_Frame();

	return NOEVENT;
}

void CPlayer::Late_Update()
{
	if (m_pCollider)
		m_pCollider->Late_Update();
}

void CPlayer::Render(HDC _dc)
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
		(int)64 * m_tFrame.iFrameStart,
		(int)64 * m_tFrame.iMotion,
		(int)64,   // 복사할 비트맵 가로 세로 사이즈
		(int)64,
		RGB(255, 255, 255));   // 제거할 픽셀 색상 값

	DebugTextOut(_dc);
}

void CPlayer::Release()
{
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
	//CCollisionManager::Collision_Rect_Resolve(GetCollider(), _pOther);

	if (CCollisionManager::Check_Mask(GetCollider(), _pOther))
	{
	}
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
	POINT vWorldMouse;
	vWorldMouse.x = g_ptMousePos.x / g_fZoom - (int)CScrollManager::Get_Instance()->Get_ScrollX();
	vWorldMouse.y = g_ptMousePos.y / g_fZoom - (int)CScrollManager::Get_Instance()->Get_ScrollY();

	if (CKeyManager::Get_Instance()->Key_Down(VK_RBUTTON) && !m_tAttackInfo.m_bIsAttack)
	{
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
	}

	if (CKeyManager::Get_Instance()->Key_Pressing('W'))
	{
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
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;
		case RUN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;
		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;
		case SKILL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;
		case ULT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;
		case DIE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;
		}

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
		keyword = L"_attck_";
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

void CPlayer::MoveVector()
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
	std::wstring wstrHP = std::to_wstring(m_iHP);
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

void CPlayer::AttackPoc()
{
	if (Get_DistToTarget() <= m_fDistance)
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
	else
		m_eCurState = RUN;

	if (m_eCurState == ATTACK)
	{
		m_tAttackInfo.m_fdtAttackTime += fDT;
		if (m_tAttackInfo.m_fdtAttackTime >= m_tAttackInfo.m_fAttackDelay)
		{
			m_tAttackInfo.m_fdtAttackTime = 0.f;
			m_tAttackInfo.m_bIsAttack = false;
		}
		else if (!m_tAttackInfo.m_bIsAttack)
		{
			static_cast<CCharacter*>(m_pTarget)->OnHit(m_tAttackInfo);
			m_tAttackInfo.m_bIsAttack = true;
		}
	}
}