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
	auto recMsg = CTcpManager::GetInstance()->ListenSocket();
	cout << recMsg << "\n";
	if (recMsg != "")
	{
		json j = recMsg;
		DTOPLAYER dto = j.get<DTOPLAYER>();

		for (auto a : CSceneManager::GetInstance()->GetCurScene()->GetObjectList()[OBJ_PLAYER])
		{
			if (!static_cast<CPlayer*>(a)->GetIsMine())
			{
				a->SetPosX(dto.fX - 10.f);
				a->SetPosY(dto.fY + 10.f);
			}
		}
	}

	//�ð� ������Ʈ
	m_fdtPlayTime += fDT;

	//Ÿ�� ������Ʈ
	CTileManager::Get_Instance()->Update();

	//Ű �Է�
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

	//Ÿ�� �ʱ�ȭ
	CTileManager::Get_Instance()->Initialize();
	//Ÿ�� �ҷ�����
	CTileManager::Get_Instance()->Load_Tile();

	//�÷��̾� �ʱ�ȭ
	CObject* pPlayer = new CPlayer();
	pPlayer->Initialize();
	pPlayer->SetPos(Vec2(50.f, 800.f));
	pPlayer->SetName(L"Player");
	static_cast<CPlayer*>(pPlayer)->SetIsMine(true);
	static_cast<CPlayer*>(pPlayer)->SetIsHost(true);
	AddObject(pPlayer, OBJ_PLAYER);
	RegisterPlayer(pPlayer);

	//�׽�Ʈ �÷��̾� �ʱ�ȭ
	CObject* pTestListener = new CPlayer();
	pTestListener->Initialize();
	pTestListener->SetPos(Vec2(150.f, 800.f));
	pTestListener->SetName(L"Player");
	static_cast<CPlayer*>(pTestListener)->SetIsMine(false);
	static_cast<CPlayer*>(pTestListener)->SetIsHost(false);
	static_cast<CPlayer*>(pTestListener)->SetTeam(false);
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
		(int)1920,	// ������ ��Ʈ�� ���� ���� ������
		(int)1080,
		RGB(255, 0, 255));	// ������ �ȼ� ���� ��
}
