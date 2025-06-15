#include "pch.h"
#include "CCollider.h"
#include "CObject.h"
#include "CScrollManager.h"
#include "CBmpManager.h"
#include "assert.h"

UINT CCollider::g_iNextID = 0;

CCollider::CCollider() : m_iID(g_iNextID++), m_iCol(0)
{

}

CCollider::CCollider(const CCollider& _origin)
{
    m_pOwner = nullptr;
    m_iID = g_iNextID;
    m_bActive = _origin.m_bActive;
    m_vOffsetPos = _origin.m_vOffsetPos;
    m_vScale = _origin.m_vScale;
}

CCollider::~CCollider()
{

}

void CCollider::Initialize(CObject* _pObj)
{
    __super::Initialize(_pObj);
}

void CCollider::Update()
{
}

void CCollider::Late_Update()
{
    // Object의 위치를 따라가겠다.
    Vec2 vObjectPos = m_pOwner->GetPos();
    m_vFinalPos = vObjectPos + m_vOffsetPos;

    assert(0 <= m_iCol);
}

void CCollider::Render(HDC _dc)
{
    if (!m_bActive || !bColRender)
        return;

    HPEN hPen = CreatePen(PS_SOLID, 1, m_iCol ? RGB(255, 0, 0) : RGB(0, 255, 0));
    HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

    HPEN hOldPen = (HPEN)SelectObject(_dc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(_dc, hBrush);

    float fScrollX = CScrollManager::Get_Instance()->Get_ScrollX();
    float fScrollY = CScrollManager::Get_Instance()->Get_ScrollY();

    Vec2 ownerPos = m_pOwner->GetPos();

    int drawX = int(ownerPos.x * g_fZoom + fScrollX * g_fZoom);
    int drawY = int(ownerPos.y * g_fZoom + fScrollY * g_fZoom);

    int spriteW = int(m_vScale.x * g_fZoom);
    int spriteH = int(m_vScale.y * g_fZoom);

    // 스크롤을 더해야 플레이어와 동일한 기준
    Vec2 vRenderPos = Vec2(drawX, drawY);

    Rectangle(_dc,
        (int)(vRenderPos.x - spriteW * 0.5f),
        (int)(vRenderPos.y - spriteH * 0.5f),
        (int)(vRenderPos.x + spriteW * 0.5f),
        (int)(vRenderPos.y + spriteH * 0.5f));

    SelectObject(_dc, hOldPen);
    SelectObject(_dc, hOldBrush);
    DeleteObject(hPen);
}

void CCollider::OnCollision(CCollider* _pOther)
{
    m_pOwner->OnCollision(_pOther);
}

void CCollider::OnCollisionEnter(CCollider* _pOther)
{
    m_pOwner->OnCollisionEnter(_pOther);
    ++m_iCol;
}

void CCollider::OnCollisionExit(CCollider* _pOther)
{
    m_pOwner->OnCollisionExit(_pOther);
    --m_iCol;
}