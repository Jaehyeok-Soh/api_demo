#pragma once

#define PURE = 0

#define WINCX 800
#define WINCY 600

#define TILECX 16
#define TILECY 16

#define TILEX 123
#define TILEY 68

#define VK_MAX 0xff

#define DEAD 0
#define NOEVENT 1

#define PI		3.141592f

#define fDT  CTimeManager::Get_Instance()->GetfDT()
#define DT CTimeManager::Get_Instance()->GetDT()


#define CheckTrue(x) { if(x == true) return; }

#define CheckFalse(x) { if(x == false) return; }

#define CheckNull(x) { if(x == nullptr) return; }

#define CheckTrueResult(x, y) { if(x == true) return y; }

#define CheckFalseResult(x, y) { if(x == false) return y;}

#define CheckNullResult(x, y) { if(x == nullptr) return y;}

extern HWND g_hWnd;
extern bool bColRender;
extern float g_fZoom;
extern bool bTileRender;
extern POINT g_ptMousePos;
extern HINSTANCE hInst;

struct INFO
{
	float fX;
	float fY;
	float fCX;
	float fCY;
};

struct FRAME
{
	int		iFrameStart;
	int		iFrameEnd;
	int		iMotion;
	DWORD	dwSpeed;
	DWORD	dwTime;

};

struct ATTACKINFO
{
	bool m_bIsAttack;
	int m_iDamage;
	float m_fdtAttackTime;
	float m_fAttackDelay;
};

enum TILETYPE
{
	PEEK_DISABLE = 0,
	PEEK_ENABLE,
	TERRAIN_BUSH,

	BLUE_SP = 10,
	BLUE_NEXUS,
	BLUE_INHIBITER,
	BLUE_TURRET1,
	BLUE_TURRET2,
	BLUE_MELEE_SP,

	RED_SP = 20,
	RED_NEXUS,
	RED_INHIBITER,
	RED_TURRET1,
	RED_TURRET2,
	RED_MELEE_SP
};

enum OBJID
{
	OBJ_PLAYER,
	OBJ_TOWER,
	OBJ_MINION,
	OBJ_ATTACK,
	OBJ_SKILL,
	OBJ_TILE,
	OBJ_UI,
	OBJ_END
};

enum SCENEID
{
	SC_MENU,
	SC_LOBBY,
	SC_LOADING,
	SC_PLAY,
	SC_EDIT,
	SC_END,
};

enum COMPONENTID
{
	COMPONENT_COLLIDER,
	COMPONENT_GRAVITY,
	COMPONENT_END
};

enum COLLAYERID
{
	COL_CHARACTER = 1 << 0,
	COL_PLAYER = 1 << 1,
	COL_ENEMY = 1 << 2,
	COL_MINION = 1 << 3,

	COL_TOWER = 1 << 4,
	COL_NEXUS = 1 << 5,
	COL_TURRET = 1 << 6,
	COL_INHIBITOR = 1 << 7,
	
	COL_ATTACK = 1 << 8,
	COL_SKILL = 1 << 9
};

#define SAFE_DELETE(x) if(x) { delete x; x = nullptr; }
#define SAFE_DELETE_ARRAY(x) if(x) { delete[] x; x = nullptr; }

extern HWND g_hWnd;

template<typename T>
void Safe_Delete(T& p)
{
	SAFE_DELETE(p);
}

template<typename T>
void Safe_Delete_Array(T& p)
{
	SAFE_DELETE_ARRAY(p);
}

struct tagFinder
{
	tagFinder(const TCHAR* pString) : m_pString(pString) {}

	template<typename T>
	bool operator()(T& MyPair)
	{
		return !lstrcmp(MyPair.first, m_pString);
	}

	const TCHAR* m_pString;
};

struct DeleteObj
{
	template<typename T>
	void operator()(T& p)
	{
		SAFE_DELETE(p);
	}
};

struct DeleteMap
{
	template<typename T>
	void operator()(T& MyPair)
	{
		if (MyPair.second)
		{
			delete MyPair.second;
			MyPair.second = nullptr;
		}
	}
};