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
#include "CGameManager.h"
#include "CBlendingManager.h"

CPlay::CPlay()
	: gameSet(false),
	win(-1),
	m_EogDC(0)
{
	ZeroMemory(&m_tEndingFrame, sizeof(FRAME));
	ZeroMemory(&m_tEogColorFrame, sizeof(FRAME));
}

CPlay::~CPlay()
{
	static_cast<CPlayer*>(CSceneManager::GetInstance()->GetPlayer())->ToDTO(false, true, "");
	CTcpManager::DestroyInstance();
	CBlendingManager::GetInstance()->Release();
}

void CPlay::Update()
{
	CGameManager::GetInstance()->Update();

	if (!gameSet)
	{
		thread syncT(&CTcpManager::SyncPlay, CTcpManager::GetInstance());
		syncT.join();
	}

	//타일 업데이트
	CTileManager::Get_Instance()->Update();

	//키 입력
	Key_Input();

	if (!gameSet)
	{
		//Late_Update
		CScene::Update();
	}

	CColliderManager::Get_Instance()->Update();

	Update_Frame();

	if (gameSet)
	{
		Update_Eog_Frame();
		if (m_tEndingFrame.iFrameStart >= 20)
		{
			Update_Eog_Color_Frame();
		}

		if (m_tEndingFrame.iFrameStart == m_tEndingFrame.iFrameEnd)
		{
			//add object eogbutton
		}
	}
}

void CPlay::Render(HDC _dc)
{
	int iScrollX = CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = CScrollManager::Get_Instance()->Get_ScrollY();

	Render_Map(_dc, iScrollX, iScrollY);

	CTileManager::Get_Instance()->Render(_dc);

	CScene::Render(_dc);

	Render_UI(_dc);

	if (gameSet)
	{
		auto scrollManager = CScrollManager::Get_Instance();
		auto team = CSceneManager::GetInstance()->GetPlayer()->GetTeam();

		if (team)
		{
			if (win)
			{
				if (scrollManager->Get_ScrollX() > -469.f)
				{
					scrollManager->Set_ScrollX(-10.f);
				}
				if (scrollManager->Get_ScrollY() < 0)
				{
					scrollManager->Set_ScrollY(10.f);
				}
			}
			else
			{
				if (scrollManager->Get_ScrollX() < -10.f)
				{
					scrollManager->Set_ScrollX(10.f);
				}
				if (scrollManager->Get_ScrollY() > -1750.f)
				{
					scrollManager->Set_ScrollY(-10.f);
				}
			}
		}
		else
		{
			if (win)
			{
				if (scrollManager->Get_ScrollX() < -10.f)
				{
					scrollManager->Set_ScrollX(10.f);
				}
				if (scrollManager->Get_ScrollY() > -1750.f)
				{
					scrollManager->Set_ScrollY(-10.f);
				}
			}
			else
			{
				if (scrollManager->Get_ScrollX() > -469.f)
				{
					scrollManager->Set_ScrollX(-10.f);
				}
				if (scrollManager->Get_ScrollY() < 0)
				{
					scrollManager->Set_ScrollY(10.f);
				}
			}
		}

		
		
		Render_Eog(_dc);

		if (m_tEndingFrame.iFrameStart >= 20)
		{
			Render_Eog_Color(_dc);
		}

		if (m_tEndingFrame.iFrameStart >= 16)
		{
			Render_Eog_Base(_dc);
		}

		if (m_tEndingFrame.iFrameStart >= 23)
		{
			Render_Eog_Title(_dc);
		}
	}
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

	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_PLAYER, OBJID::OBJ_BUSH);

	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_MINION, OBJID::OBJ_TOWER);
	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_MINION, OBJID::OBJ_MINION);
	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_MINION, OBJID::OBJ_ATTACK);
	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_MINION, OBJID::OBJ_SKILL);

	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_MINION, OBJID::OBJ_BUSH);

	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_TOWER, OBJID::OBJ_ATTACK);
	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_TOWER, OBJID::OBJ_SKILL);

	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_HITBOX, OBJID::OBJ_MINION);
	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_HITBOX, OBJID::OBJ_PLAYER);

	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_WALL, OBJID::OBJ_PLAYER);
	CColliderManager::Get_Instance()->CheckGroup(OBJID::OBJ_WALL, OBJID::OBJ_MINION);

	Initialize();
}

void CPlay::Exit()
{
	DeleteAll();
	CColliderManager::Get_Instance()->Reset();
}

void CPlay::Initialize()
{
	CBlendingManager::GetInstance()->Initialize();

	CSceneManager::GetInstance()->SetChangeScene(false, SC_PLAY);

	static_cast<CPlayer*>(CSceneManager::GetInstance()->GetPlayer())->ToDTO(true, false, "");

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
		CScrollManager::Get_Instance()->Set_ScrollX(-469.f);
		CScrollManager::Get_Instance()->Set_ScrollY(0.f);
	}

	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iMotion = 0;

	m_EogDC = CBmpManager::Get_Instance()->Find_Image(L"eog");

	m_tEndingFrame.dwSpeed = 50;
	m_tEndingFrame.dwTime;
	m_tEndingFrame.iFrameStart = 0;
	m_tEndingFrame.iFrameEnd = 44;
	m_tEndingFrame.iMotion = 0;

	m_tEogColorFrame.dwSpeed = 50;
	m_tEogColorFrame.dwTime;
	m_tEogColorFrame.iFrameStart = 0;
	m_tEogColorFrame.iFrameEnd = 2;
	m_tEogColorFrame.iMotion = 0;
}

void CPlay::Key_Input()
{
	if (gameSet)
		return;
	
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
		720,
		405,
		RGB(255, 0, 255));
}

void CPlay::Render_UI(HDC hdc)
{
	CBlendingManager::GetInstance()->RenderBlend(hdc, L"../Image/ApiDemo/Client/scoreBoard.png", Rect(435, 0, 510, 54), 0, 0, 510, 54, 1.f);

	HDC minimapBorder = CBmpManager::Get_Instance()->Find_Image(L"minimapBorder");
	HDC Minimap = CBmpManager::Get_Instance()->Find_Image(L"Minimap");
	GdiTransparentBlt(minimapBorder,
		10,
		10,
		290,
		290,
		Minimap,
		0,
		0,
		512,
		512,
		RGB(1, 1, 1));

	GdiTransparentBlt(hdc,
		1080,
		520,
		200,
		200,
		minimapBorder,
		0,
		0,
		310,
		310,
		RGB(255, 255, 255));

	CBlendingManager::GetInstance()->RenderBlend(hdc, L"../Image/ApiDemo/Client/HUD/playBottom.png", Rect(140, 550, 900, 170), 0, 0, 900, 170, 1.f);

	int iHp = static_cast<CPlayer*>(CSceneManager::GetInstance()->GetPlayer())->GetStatus().m_iHp;
	HDC bar_big1 = CBmpManager::Get_Instance()->Find_Image(L"bar_big1");
	GdiTransparentBlt(hdc,
		135 + 171,
		551 + 112,
		(int)(467.f * ((float)iHp / 1000.f)),
		43,
		bar_big1,
		58 * m_tFrame.iFrameStart,
		0,
		58,
		43,
		RGB(1, 1, 1));

	CBlendingManager::GetInstance()->RenderBlend(hdc,
		L"../Image/ApiDemo/Client/HUD/bar_big_marker.png",
		Rect(773 - (467 - (int)(467.f * ((float)iHp / 1000.f))) - 12, 641, 24, 88),
		0,
		0,
		12,
		44,
		1.f);
}

void CPlay::Render_Eog(HDC hdc)
{
	wstring _path;

	if (m_tEndingFrame.iFrameStart >= 0 && m_tEndingFrame.iFrameStart <= 16)
	{
		_path = L"../Image/ApiDemo/Client/endofgame/eog_fb/eog_fb_color_01_";
		if (m_tEndingFrame.iFrameStart + 1 < 10)
			_path += to_wstring(0);
		_path += to_wstring(m_tEndingFrame.iFrameStart + 1);
		_path += L".png";
	}
	else if (m_tEndingFrame.iFrameStart >= 16 && m_tEndingFrame.iFrameStart <= 32)
	{
		_path = L"../Image/ApiDemo/Client/endofgame/eog_fb/eog_fb_color_02_";
		if (m_tEndingFrame.iFrameStart - 15 < 10)
			_path += to_wstring(0);
		_path += to_wstring(m_tEndingFrame.iFrameStart - 15);
		_path += L".png";
	}
	else if (m_tEndingFrame.iFrameStart >= 33 && m_tEndingFrame.iFrameStart <= 45)
	{
		_path = L"../Image/ApiDemo/Client/endofgame/eog_fb/eog_fb_color_03_";
		if (m_tEndingFrame.iFrameStart - 32 < 10)
			_path += to_wstring(0);
		_path += to_wstring(m_tEndingFrame.iFrameStart - 32);
		_path += L".png";
	}

	//CBlendingManager::GetInstance()->RenderBlend(hdc, _path, Rect(0, 0, WINCX, WINCY), 0, 0, 683, 683, 0.5f);

	thread i1(&CBlendingManager::RenderBlend, CBlendingManager::GetInstance(), hdc, _path, Rect(0, 0, WINCX, WINCY), 0, 0, 683, 683, 0.5f);
	i1.join();
}

void CPlay::Render_Eog_Base(HDC hdc)
{
	if (win)
	{
		//CBlendingManager::GetInstance()->RenderBlend(hdc, L"../Image/ApiDemo/Client/endofgame/eog_base.png", Rect(400, 200, 512, 256), 0, 0, 512, 256, 1.f);

		thread i1(&CBlendingManager::RenderBlend, CBlendingManager::GetInstance(), hdc, L"../Image/ApiDemo/Client/endofgame/eog_base.png", Rect(400, 200, 512, 256), 0, 0, 512, 256, 1.f);
		i1.join();
	}
	else
	{
		CBlendingManager::GetInstance()->RenderBlend(hdc, L"../Image/ApiDemo/Client/endofgame/eog_defeat_base.png", Rect(400, 200, 512, 256), 0, 0, 512, 256, 1.f);
	}
}

void CPlay::Render_Eog_Title(HDC hdc)
{
	if (win)
	{
		CBlendingManager::GetInstance()->RenderBlend(hdc, L"../Image/ApiDemo/Client/endofgame/eog_victory.png", Rect(400, 200, 512, 256), 0, 0, 512, 256, 1.f);
	}
	else
	{
		CBlendingManager::GetInstance()->RenderBlend(hdc, L"../Image/ApiDemo/Client/endofgame/eog_defeat.png", Rect(400, 200, 512, 256), 0, 0, 512, 256, 1.f);
	}
}

void CPlay::Render_Eog_Color(HDC hdc)
{
	CBlendingManager::GetInstance()->Render(hdc, L"../Image/ApiDemo/Client/endofgame/eog_fb_color.png", Rect(0, 0, WINCX, WINCY), 682 * m_tEogColorFrame.iFrameStart, 682 * m_tEogColorFrame.iMotion, 682, 682, 0.6f);
}

void CPlay::Update_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			m_tFrame.iFrameStart = 0;
	}
}

void CPlay::Update_Eog_Frame()
{
	if (m_tEndingFrame.dwTime + m_tEndingFrame.dwSpeed < GetTickCount())
	{
		++m_tEndingFrame.iFrameStart;
		m_tEndingFrame.dwTime = GetTickCount();

		if (m_tEndingFrame.iFrameStart > m_tEndingFrame.iFrameEnd)
		{
			m_tEndingFrame.iFrameStart = 33;
		}
	}
}

void CPlay::Update_Eog_Color_Frame()
{
	if (m_tEogColorFrame.dwTime + m_tEogColorFrame.dwSpeed < GetTickCount())
	{
		++m_tEogColorFrame.iFrameStart;
		m_tEogColorFrame.dwTime = GetTickCount();

		if (m_tEogColorFrame.iFrameStart > m_tEogColorFrame.iFrameEnd)
		{
			m_tEogColorFrame.iMotion++;

			if (win && m_tEogColorFrame.iMotion > 3)
			{
				m_tEogColorFrame.iFrameStart = 0;
				m_tEogColorFrame.iMotion = 2;
			}

			if (!win && m_tEogColorFrame.iMotion > 5 && m_tEogColorFrame.iFrameStart > 0)
			{
				m_tEogColorFrame.iFrameStart = 1;
				m_tEogColorFrame.iMotion = 4;
			}

			m_tEogColorFrame.iFrameStart = 0;
		}
	}
}
