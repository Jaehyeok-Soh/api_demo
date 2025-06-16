#include "pch.h"
#include "CMinion.h"
#include "CScrollManager.h"
#include "CTimeManager.h"
#include "CBmpManager.h"
#include "CTileManager.h"
#include "CAStarManager.h"

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
	GetCollider()->SetOffsetPos(Vec2(0.f, 65.f));
	GetCollider()->SetScale(Vec2(8.f, 8.f));

	m_iHP = 100;

	m_vScale = { 8.f, 8.f };
	m_fSpeed = 100.f;

	m_eCurState = MOVE;
	m_ePreState = END;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 200;

	//move to center
	Vec2 startIdx;
	Vec2 endIdx;
 	if (m_bTeam)
	{
		startIdx = Vec2(13.f, 44.f);
		endIdx = Vec2(96.f, 13.f);
		m_vMoveDir.x = 1.f;
	}
	else
	{
		startIdx = Vec2(96.f, 13.f);
		endIdx = Vec2(13.f, 44.f);
		m_vMoveDir.x = -1.f;
	}
	

	auto future = async(launch::async,
		&CAStarManager::FindPath,
		CAStarManager::GetInstance(),
		startIdx, endIdx);

	m_Path = future.get();

	if (!m_Path.empty())
	{
		m_Path.pop_front();
		m_eCurState = MOVE;
	}
}

int CMinion::Update()
{
	if (m_eCurState == MOVE)
	{
		thread t1(&CMinion::MoveVector, this);
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
}


void CMinion::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
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

void CMinion::MoveVector()
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
	std::wstring wstrHP = std::to_wstring(m_iHP);
	LPCWSTR szHP = wstrHP.c_str();
	TextOut(_dc,
		(int)drawX + 60,
		(int)drawY - 70,
		szHP,
		lstrlen(szHP));
#pragma endregion
}

