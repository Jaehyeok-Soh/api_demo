#include "pch.h"
#include "CBullet.h"
#include "CTimeManager.h"
#include "CScrollManager.h"
#include "CBmpManager.h"

CBullet::CBullet()
{
}

CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize()
{
	m_fSpeed = 100.f;
}

void CBullet::Initialize(CObject* _pTarget)
{
	Initialize();

	m_pTarget = _pTarget;

	CreateCollider();

	GetCollider()->SetScale(Vec2(8.f, 8.f));
	GetCollider()->Set_Layer(COL_ATTACK);
	GetCollider()->Set_Mask(COL_MINION
		| COL_TOWER
		| COL_PLAYER);
}

int CBullet::Update()
{
	if (m_bDead)
		return DEAD;

	CalcAngle();
	MoveToAngle();

	__super::Update_Rect();

	return NOEVENT;
}

void CBullet::Late_Update()
{
	if (m_pCollider)
		m_pCollider->Late_Update();
}

void CBullet::Render(HDC _dc)
{
	if (m_vMoveDir.x > 0)
		m_pFrameKey = L"acher_arrow_r";
	else
		m_pFrameKey = L"acher_arrow_l";

	Component_Render(_dc);
	int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	int drawX = int(m_vPos.x * g_fZoom + iScrollX * g_fZoom);
	int drawY = int(m_vPos.y * g_fZoom + iScrollY * g_fZoom);

	int spriteW = int(m_vScale.x * g_fZoom);
	int spriteH = int(m_vScale.y * g_fZoom);

	HDC   hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(_dc,
		drawX - (spriteW / 2),
		drawY - (spriteH / 2),
		spriteW,
		spriteH,
		hMemDC,
		0,
		0,
		16,   // º¹»çÇÒ ºñÆ®¸Ê °¡·Î ¼¼·Î »çÀÌÁî
		16,
		RGB(0, 0, 0));   // Á¦°ÅÇÒ ÇÈ¼¿ »ö»ó °ª

#pragma region ÇöÀçÁÂÇ¥x
	std::wstring posX = std::to_wstring(m_vPos.x);
	LPCWSTR szPosX = posX.c_str();
	TextOut(_dc,
		(int)drawX - 30,
		(int)drawY - 90,
		szPosX,
		lstrlen(szPosX));
#pragma endregion
#pragma region ÇöÀçÁÂÇ¥Y
	std::wstring posY = std::to_wstring(m_vPos.y);
	LPCWSTR szPosY = posY.c_str();
	TextOut(_dc,
		(int)drawX + 30,
		(int)drawY - 90,
		szPosY,
		lstrlen(szPosY));
#pragma endregion
#pragma region ·£´õÁÂÇ¥x
	std::wstring RposX = std::to_wstring(drawX - spriteW / 2);
	LPCWSTR szRPosX = RposX.c_str();
	TextOut(_dc,
		(int)drawX - 30,
		(int)drawY - 110,
		szRPosX,
		lstrlen(szRPosX));
#pragma endregion
#pragma region ·»´õÁÂÇ¥Y
	std::wstring RposY = std::to_wstring(drawY - spriteH / 2);
	LPCWSTR szRPosY = RposY.c_str();
	TextOut(_dc,
		(int)drawX + 30,
		(int)drawY - 110,
		szRPosY,
		lstrlen(szRPosY));
#pragma endregion
}

void CBullet::Release()
{
}

void CBullet::CalcAngle()
{
	float m_fWidth = m_vPos.x - m_pTarget->GetPos().x;
	float m_fHeight = m_vPos.y - m_pTarget->GetPos().y;
	float m_fDist = sqrt(m_fWidth * m_fWidth + m_fHeight * m_fHeight);
	m_fAngle = acosf(m_fWidth/m_fDist);
	
	if (m_vPos.y <= m_pTarget->GetPos().y)
		m_fAngle *= -1.f;
}

void CBullet::MoveToAngle()
{
	m_vMoveDir.x = cosf(RADTODEG(m_fAngle)) > 0 ? 1 : -1;
	
	m_vPos.x += ((m_fSpeed * cosf(RADTODEG(m_fAngle))) * fDT);
	m_vPos.y += ((m_fSpeed * sinf(RADTODEG(m_fAngle))) * fDT);
}
