#include "pch.h"
#include "CPlayer.h"
#include "CKeyManager.h"
#include "CScrollManager.h"
#include "CTimeManager.h"

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
}

int CPlayer::Update()
{
	return 0;
}

void CPlayer::Late_Update()
{
}

void CPlayer::Render(HDC _dc)
{
}

void CPlayer::Release()
{
}

void CPlayer::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LEFT))
	{
	}

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_RIGHT))
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
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 1;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;
		}

		m_ePreState = m_eCurState;

	}
}
