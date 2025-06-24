#include "pch.h"
#include "CPlay.h"
#include "CTileManager.h"
#include "CScrollManager.h"
#include "CKeyManager.h"
#include "CBmpManager.h"
#include "CColliderManager.h"
#include "CSceneManager.h"
#include "CObjectManager.h"
#include "CTimeManager.h"
#include "CPeekingManager.h"
#include "CTcpManager.h"
#include "DTOPLAYER.h"

CPlay::CPlay()
	: m_fdtPlayTime(0.f)
{
}

CPlay::~CPlay()
{
}

void CPlay::Update()
{
	//auto recMsg = CTcpManager::GetInstance()->ListenSocket();
	//CTcpManager::GetInstance()->SyncPlay

	thread syncT(&CTcpManager::SyncPlay, CTcpManager::GetInstance());
	syncT.join();

	//CTcpManager::GetInstance()->SyncPlay();

	//시간 업데이트
	m_fdtPlayTime += fDT;

	//타일 업데이트
	CTileManager::Get_Instance()->Update();

	//키 입력
	Key_Input();

	//Late_Update
	CTileManager::Get_Instance()->Late_Update();
	CScene::Update();

	//TODO: DTO Player to json string serialize
}

void CPlay::Render(HDC _dc)
{
	int iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = CScrollManager::Get_Instance()->Get_ScrollY();

	Render_Map(_dc, iScrollX, iScrollY);

	CTileManager::Get_Instance()->Render(_dc);

	CScene::Render(_dc);
}

void CPlay::Enter()
{
	// StageManager로부터 기존 Player 가져오기
	CObject* pPlayer = CSceneManager::GetInstance()->GetPlayer();

	if (pPlayer)
	{
		AddObject(pPlayer, OBJID::OBJ_PLAYER);  // 오브젝트 리스트에 다시 추가
		RegisterPlayer(pPlayer);               // 충돌 및 중심 참조 등록
	}

	list<CObject*> otherPlayers = CSceneManager::GetInstance()->GetOtherPlayer();
	for (auto p : otherPlayers)
	{
		AddObject(p, OBJID::OBJ_PLAYER);  // 오브젝트 리스트에 다시 추가
	}

	CObject* pWeapon = CSceneManager::GetInstance()->GetWeapon();

	if (pWeapon)
	{
		AddObject(pWeapon, OBJID::OBJ_WEAPON);  // 오브젝트 리스트에 다시 추가
		RegisterWeapon(pWeapon);
	}

	list<CObject*> otherWeapons = CSceneManager::GetInstance()->GetOtherWeapon();
	for (auto p : otherWeapons)
	{
		AddObject(p, OBJID::OBJ_WEAPON);  // 오브젝트 리스트에 다시 추가
	}

	MapDC = CBmpManager::Get_Instance()->Find_Image(L"MapBig");
	SetMaxScroll(L"MapBig");

	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_PLAYER, OBJID::OBJ_TOWER);
	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_PLAYER, OBJID::OBJ_PLAYER);
	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_PLAYER, OBJID::OBJ_MINION);
	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_PLAYER, OBJID::OBJ_SKILL);
	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_PLAYER, OBJID::OBJ_ATTACK);

	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_MINION, OBJID::OBJ_TOWER);
	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_MINION, OBJID::OBJ_MINION);
	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_MINION, OBJID::OBJ_ATTACK);
	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_MINION, OBJID::OBJ_SKILL);

	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_TOWER, OBJID::OBJ_ATTACK);
	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_TOWER, OBJID::OBJ_SKILL);

	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_HITBOX, OBJID::OBJ_MINION);
	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_HITBOX, OBJID::OBJ_PLAYER);

	Initialize();
}

void CPlay::Exit()
{
	DeleteAll();
	CColliderManager::Get_Instance()->Reset();
}

void CPlay::Initialize()
{
	/*thread syncT([]() {
		while (true)
		{
			CTcpManager::GetInstance()->SyncPlay();
			this_thread::sleep_for(10ms);
		}
	});
	syncT.detach();*/

	//CTcpManager::GetInstance()->Initialize();
	CSceneManager::GetInstance()->SetChangeScene(false, SC_PLAY);

	CTcpManager::GetInstance()->SendSocket("true");

	//타일 초기화
	CTileManager::Get_Instance()->Initialize();
	//타일 불러오기
	CTileManager::Get_Instance()->Load_Tile();

	CPeekingManager::GetInstance()->Initialize();

	if (m_pPlayer->GetTeam())
	{
		CScrollManager::Get_Instance()->Set_ScrollX(-10.f);
		CScrollManager::Get_Instance()->Set_ScrollY(-750.f);
	}
	else
	{
		CScrollManager::Get_Instance()->Set_ScrollX(-10.f);
		CScrollManager::Get_Instance()->Set_ScrollY(-750.f);
	}
}

void CPlay::Key_Input()
{
	if (g_ptMousePos.x <= 10 && g_ptMousePos.x >= -10)
		CScrollManager::Get_Instance()->Set_ScrollX(10.f);

	if (g_ptMousePos.x >= WINCX - 10 && g_ptMousePos.x <= WINCX + 10)
		CScrollManager::Get_Instance()->Set_ScrollX(-5.f);

	if (g_ptMousePos.y <= 10 && g_ptMousePos.y >= -10)
		CScrollManager::Get_Instance()->Set_ScrollY(10.f);

	if (g_ptMousePos.y >= WINCY - 10 && g_ptMousePos.y <= WINCY + 10)
		CScrollManager::Get_Instance()->Set_ScrollY(-5.f);
}

void CPlay::Render_Map(HDC hdc, int iScrollX, int iScrollY)
{
	int iDestW = (int)(720 * g_fZoom);
	int iDestH = (int)(405 * g_fZoom);

	GdiTransparentBlt(hdc,
		(int)(iScrollX * g_fZoom),
		(int)(iScrollY * g_fZoom),
		iDestW,
		iDestH,
		MapDC,
		0,
		0,
		720,	// 복사할 비트맵 가로 세로 사이즈
		405,
		RGB(255, 0, 255));	// 제거할 픽셀 색상 값
}
