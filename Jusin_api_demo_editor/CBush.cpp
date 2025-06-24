#include "pch.h"
#include "CBush.h"
#include "CSceneManager.h"
#include "CCharacter.h"
#include "CScrollManager.h"
#include "CBmpManager.h"

CBush::CBush()
{
}

CBush::~CBush()
{
}

void CBush::Initialize()
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(4.f, 4.f));

	//m_pFrameKey = L"hideonbush";
}

int CBush::Update()
{
	__super::Update_Rect();
	return NOEVENT;
}

void CBush::Late_Update()
{
	if (m_pCollider)
	{
		m_pCollider->Late_Update();
	}
}

void CBush::Render(HDC _dc)
{
	Component_Render(_dc);

	//int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	//int iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	//int drawX = int(m_vPos.x * g_fZoom + iScrollX * g_fZoom);
	//int drawY = int(m_vPos.y * g_fZoom + iScrollY * g_fZoom);

	//int spriteW = int(20 * g_fZoom);
	//int spriteH = int(20 * g_fZoom);

	//HDC   hMemDC = CBmpManager::Get_Instance()->Find_Image(m_pFrameKey);

	//GdiTransparentBlt(_dc,
	//	drawX - spriteW / 2,
	//	drawY - spriteH / 2,
	//	spriteW,
	//	spriteH,
	//	hMemDC,
	//	0,
	//	0,
	//	(int)703,   // 복사할 비트맵 가로 세로 사이즈
	//	(int)700,
	//	RGB(255, 255, 255));   // 제거할 픽셀 색상 값
}

void CBush::Release()
{
}

void CBush::OnCollisionEnter(CCollider* _pOther)
{
	CCharacter* pObj = static_cast<CCharacter*>(_pOther->GetOwner());
	pObj->SetIsHide(true);
	pObj->SetHideOption(m_iOption);
}

void CBush::OnCollision(CCollider* _pOther)
{
	CCharacter* pObj = static_cast<CCharacter*>(_pOther->GetOwner());
	pObj->SetIsHide(true);
	pObj->SetHideOption(m_iOption);
}

void CBush::OnCollisionExit(CCollider* _pOther)
{
	CCharacter* pObj = static_cast<CCharacter*>(_pOther->GetOwner());
	pObj->SetIsHide(false);
	pObj->SetHideOption(0);
}
