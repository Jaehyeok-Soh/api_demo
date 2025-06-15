#include "pch.h"
#include "CPlayer.h"
#include "CKeyManager.h"
#include "CScrollManager.h"
#include "CTimeManager.h"
#include "CBmpManager.h"
#include "CTileManager.h"

CPlayer::CPlayer()
	: m_bIsMine(false), m_bIsHost(false)
{
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	CreateColider();

	//CreateGravity();
	GetColider()->SetOffsetPos(Vec2(0.f, 65.f));
	GetColider()->SetScale(Vec2(60.f, 60.f));

	m_iHP = 100;

	m_vPos = Vec2{ 100, 1000 };
	m_vScale = { 32.f, 32.f };
	m_fSpeed = 300.f;
	m_vMoveDir.x = 1.f;

	m_pFrameKey = L"swordman_idle_r";

	m_eCurState = IDLE;
	m_ePreState = END;
	m_eJob = SWORDMAN;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 200;

}

int CPlayer::Update()
{
	Key_Input();
	
	if (m_eCurState == RUN)
		MoveVector();
	SetFrameKey();

	Motion_Change();
	
	__super::Update_Rect();
	__super::Update_Frame();

	return NOEVENT;
}

void CPlayer::Late_Update()
{
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
		drawX - spriteW / 2,//m_tRect.left + iScrollX / g_fZoom,//
		drawY - spriteH / 2,//m_tRect.top + iScrollY / g_fZoom,//
		spriteW,//(int)m_vScale.x,//
		spriteH,//(int)m_vScale.y,//
		hMemDC,
		(int)64 * m_tFrame.iFrameStart,
		(int)64 * m_tFrame.iMotion,
		(int)64,   // ������ ��Ʈ�� ���� ���� ������
		(int)64,
		RGB(255, 255, 255));   // ������ �ȼ� ���� ��

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
}

void CPlayer::Key_Input()
{
	POINT vWorldMouse;
	vWorldMouse.x = g_ptMousePos.x / g_fZoom - (int)CScrollManager::Get_Instance()->Get_ScrollX();
	vWorldMouse.y = g_ptMousePos.y / g_fZoom - (int)CScrollManager::Get_Instance()->Get_ScrollY();

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_RBUTTON))
	{
		if (CTileManager::Get_Instance()->Peeking_Tile(vWorldMouse))
		{
			//TODO: Ÿ�� �浹 / ������Ʈ ���� ����
			// ��� �� �� ������Ʈ�� üũ���� �� ����
			//m_vDestination = Vec2((float)ptOldMouse.x, (float)ptOldMouse.y);
			m_vDestination = Vec2((float)vWorldMouse.x, (float)vWorldMouse.y);
			m_vMoveDir = m_vDestination - m_vPos;
			m_vMoveDir.Normalize();

			m_eCurState = RUN;
		}
	}

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
	}


}

void CPlayer::Offset()
{

	//int iCenterX = WINCX >> 1;
	//int   iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	//int   iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	//float fPlayerScreenX = m_vPos.x + iScrollX;
	//float fPlayerScreenY = m_vPos.y + iScrollY;

	//const float DEADZONE_X = 10.f; // X�� ������
	//const float DEADZONE_Y = 20.f; // Y�� ������

	//if (CScrollManager::Get_Instance()->Get_ScrollXMax() >= m_vPos.x + iScrollX)
	//{
	//	// X�� �߽� ����
	//	if (fPlayerScreenX < iCenterX - DEADZONE_X)
	//		CScrollManager::Get_Instance()->Set_ScrollX(m_fSpeed * fDT);

	//	else if (fPlayerScreenX > iCenterX + DEADZONE_X)
	//		CScrollManager::Get_Instance()->Set_ScrollX(-m_fSpeed * fDT);
	//}

	//int   iOffsetminY = 300;
	//int   iOffsetmaxY = 400;

	//if (CScrollManager::Get_Instance()->Get_ScrollYMax() >= m_vPos.y + iScrollY)
	//{
	//	if (iOffsetminY > m_vPos.y + iScrollY)
	//		CScrollManager::Get_Instance()->Set_ScrollY(m_fSpeed * fDT);

	//	if (iOffsetmaxY < m_vPos.y + iScrollY)
	//		CScrollManager::Get_Instance()->Set_ScrollY(-m_fSpeed * fDT);
	//}
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
	Vec2 diff = m_vDestination - m_vPos;
	float dist = diff.Length();
	float step = m_fSpeed * fDT;

	if (dist <= step)
	{
		m_vPos = m_vDestination;
		m_eCurState = IDLE;
		return;
	}

	m_vPos += m_vMoveDir * step;
}

void CPlayer::DebugTextOut(HDC _dc)
{
	int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	int drawX = int(m_vPos.x * g_fZoom + iScrollX);
	int drawY = int(m_vPos.y * g_fZoom + iScrollY);

	int spriteW = int(m_vScale.x * g_fZoom);
	int spriteH = int(m_vScale.y * g_fZoom);

#pragma region �׽�Ʈ��
	LPCWSTR szState = GetStateName(m_eCurState);
	TextOut(_dc,
		(int)m_vPos.x + iScrollX - 30,
		(int)m_vPos.y + iScrollY - 70,
		szState,
		lstrlen(szState));
#pragma endregion
#pragma region �׽�Ʈ��
	std::wstring wstrHP = std::to_wstring(m_iHP);
	LPCWSTR szHP = wstrHP.c_str();
	TextOut(_dc,
		(int)m_vPos.x + iScrollX + 60,
		(int)m_vPos.y + iScrollY - 70,
		szHP,
		lstrlen(szHP));
#pragma endregion
#pragma region ������ǥx
	std::wstring posX = std::to_wstring(m_vPos.x);
	LPCWSTR szPosX = posX.c_str();
	TextOut(_dc,
		(int)m_vPos.x + iScrollX - 30,
		(int)m_vPos.y + iScrollY - 90,
		szPosX,
		lstrlen(szPosX));
#pragma endregion
#pragma region ������ǥY
	std::wstring posY = std::to_wstring(m_vPos.y);
	LPCWSTR szPosY = posY.c_str();
	TextOut(_dc,
		(int)m_vPos.x + iScrollX + 30,
		(int)m_vPos.y + iScrollY - 90,
		szPosY,
		lstrlen(szPosY));
#pragma endregion
#pragma region ������ǥx
	std::wstring RposX = std::to_wstring(drawX - spriteW / 2);
	LPCWSTR szRPosX = RposX.c_str();
	TextOut(_dc,
		(int)m_vPos.x + iScrollX - 30,
		(int)m_vPos.y + iScrollY - 110,
		szRPosX,
		lstrlen(szRPosX));
#pragma endregion
#pragma region ������ǥY
	std::wstring RposY = std::to_wstring(drawY - spriteH / 2);
	LPCWSTR szRPosY = RposY.c_str();
	TextOut(_dc,
		(int)m_vPos.x + iScrollX + 30,
		(int)m_vPos.y + iScrollY - 110,
		szRPosY,
		lstrlen(szRPosY));
#pragma endregion
}
