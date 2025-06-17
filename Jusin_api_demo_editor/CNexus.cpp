#include "pch.h"
#include "CNexus.h"
#include "CScrollManager.h"
#include "CBmpManager.h"
#include "CTimeManager.h"
#include "CSceneManager.h"
#include "CMinion.h"

CNexus::CNexus()
	: strFrameBlueNexus(L"BlueNexus"), strFrameRedNexus(L"RedNexus"), m_fSpawnCoolDownTime(0.f)
{
}

CNexus::~CNexus()
{
}

void CNexus::Initialize()
{
	__super::Initialize();

	CreateCollider();
	//CreateGravity();

	GetCollider()->SetOffsetPos(Vec2(0.f, 65.f));
	GetCollider()->SetScale(Vec2(64.f, 64.f));
	GetCollider()->Set_Layer(COL_TOWER);
	GetCollider()->Set_Mask(COL_MINION
		| COL_TOWER
		| COL_ATTACK
		| COL_PLAYER
		| COL_SKILL);

	m_pFrameKey = L"Towers";

	m_iHP = 100;
	m_fSpawnCoolDownTime = 30.f;
}

int CNexus::Update()
{
	m_fSpawnCoolDownTime += fDT;
	//TODO: 일정시간마다 미니언 소환
	//TODO: 풀링?

	SpawnMinion();

	return NOEVENT;
}

void CNexus::Late_Update()
{
	if (m_pCollider)
		m_pCollider->Late_Update();

	/*if (m_pGravity)
		m_pGravity->Late_Update();*/
}

void CNexus::Render(HDC _dc)
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
		m_pFrameKey = strFrameBlueNexus;
		HDC hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);

		auto a = GdiTransparentBlt(_dc,
			drawX - spriteW / 2,
			drawY - spriteH / 2,
			spriteW,
			spriteH,
			hMemDC,
			0,
			0,
			157,   // 복사할 비트맵 가로 세로 사이즈
			168,
			RGB(189, 189, 189));   // 제거할 픽셀 색상 값
		auto b = 1;
	}
	else
	{
		m_pFrameKey = strFrameRedNexus;
		HDC hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);
		GdiTransparentBlt(_dc,
			drawX - spriteW / 2,
			drawY - spriteH / 2,
			spriteW,
			spriteH,
			hMemDC,
			0,
			0,
			161,   // 복사할 비트맵 가로 세로 사이즈
			208,
			RGB(189, 189, 189));   // 제거할 픽셀 색상 값
	}
}

void CNexus::Release()
{
}

void CNexus::SpawnMinion()
{
	if (m_fSpawnCoolDownTime >= 10.f)
	{
		auto curScene = CSceneManager::GetInstance()->GetCurScene();

		if (m_fSpawnTime >= m_fSpawnDelay)
		{
			CObject* pObj = new CMinion();
			auto minion = static_cast<CMinion*>(pObj);
			minion->SetPos(m_vSpawnPos);
			minion->SetTeam(m_bTeam);
			minion->Initialize();
			minion->SetName(L"Minion");
			curScene->AddObject(minion, OBJ_MINION);

			m_fSpawnTime = fDT;
			m_iSpawnCnt++;
			if (m_iSpawnCnt >= m_fSpawnMaxOnTime)
			{
				m_iSpawnCnt = 0;
				m_fSpawnCoolDownTime = fDT;
			}
		}

		m_fSpawnTime += fDT;
	}
}
