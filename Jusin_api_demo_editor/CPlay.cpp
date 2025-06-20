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
	thread tListen(&CTcpManager::ListenSocket, CTcpManager::GetInstance());
	tListen.detach();

	auto recMsg = CTcpManager::GetInstance()->ListenSocket();
	cout << recMsg << "\n";
	if (recMsg != "")
	{
		try
		{
			nlohmann::json j = nlohmann::json::parse(recMsg);
			DTOPLAYER dto = j.get<DTOPLAYER>();

			for (auto a : CSceneManager::GetInstance()->GetCurScene()->GetObjectList()[OBJ_PLAYER])
			{
				if (!static_cast<CPlayer*>(a)->GetIsMine())
				{
					a->SetPosX(dto.fX - 10.f);
					a->SetPosY(dto.fY + 10.f);
					static_cast<CPlayer*>(a)->SetState((CCharacter::STATE)dto.m_iState);
					static_cast<CPlayer*>(a)->SetFrameStart(dto.m_iFrameStart);

					wstring wstr(dto.m_strFrameKey.begin(), dto.m_strFrameKey.end());
					static_cast<CPlayer*>(a)->Set_FrameKey(wstr.c_str());

					static_cast<CPlayer*>(a)->SetDirection(dto.m_iDir);
				}
			}
		}
		catch (const std::exception&)
		{

		}
	}

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
 	CTcpManager::GetInstance()->Initialize();
	CSceneManager::GetInstance()->SetChangeScene(false, SC_EDIT);

	//타일 초기화
	CTileManager::Get_Instance()->Initialize();
	//타일 불러오기
	CTileManager::Get_Instance()->Load_Tile();

	//플레이어 초기화
	CObject* pPlayer = new CPlayer();
	pPlayer->SetPos(Vec2(50.f, 800.f));
	pPlayer->SetName(L"Player");
	static_cast<CPlayer*>(pPlayer)->SetIsMine(true);
	static_cast<CPlayer*>(pPlayer)->SetIsHost(true);
	pPlayer->Initialize();
	AddObject(pPlayer, OBJ_PLAYER);
	RegisterPlayer(pPlayer);

	//테스트 플레이어 초기화
	CObject* pTestListener = new CPlayer();
	pTestListener->SetPos(Vec2(150.f, 800.f));
	pTestListener->SetName(L"Player");
	static_cast<CPlayer*>(pTestListener)->SetIsMine(false);
	static_cast<CPlayer*>(pTestListener)->SetIsHost(false);
	static_cast<CPlayer*>(pTestListener)->SetTeam(false);
	pTestListener->Initialize();
	AddObject(pTestListener, OBJ_PLAYER);

	CPeekingManager::GetInstance()->Initialize();
}

void CPlay::Key_Input()
{
	if (g_ptMousePos.x <= 10)
		CScrollManager::Get_Instance()->Set_ScrollX(10.f);

	if (g_ptMousePos.x >= WINCX - 10)
		CScrollManager::Get_Instance()->Set_ScrollX(-10.f);

	if (g_ptMousePos.y <= 10)
		CScrollManager::Get_Instance()->Set_ScrollY(10.f);

	if (g_ptMousePos.y >= WINCY - 10)
		CScrollManager::Get_Instance()->Set_ScrollY(-10.f);
}

void CPlay::Render_Map(HDC hdc, int iScrollX, int iScrollY)
{
	int iDestW = (int)(1920 * g_fZoom);
	int iDestH = (int)(1080 * g_fZoom);

	GdiTransparentBlt(hdc,
		(int)(iScrollX * g_fZoom),
		(int)(iScrollY * g_fZoom),
		iDestW,
		iDestH,
		MapDC,
		0,
		0,
		(int)1920,	// 복사할 비트맵 가로 세로 사이즈
		(int)1080,
		RGB(255, 0, 255));	// 제거할 픽셀 색상 값
}
