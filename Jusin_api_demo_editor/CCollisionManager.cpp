#include "pch.h"
#include "CCollisionManager.h"
#include "CCollider.h"
#include "CTimeManager.h"

void CCollisionManager::Collision_Rect_Resolve(CComponent* _Dst, CComponent* _Src)
{

	auto pDstCollider = dynamic_cast<CCollider*>(_Dst);
	auto pSrcCollider = dynamic_cast<CCollider*>(_Src);

	if (!pDstCollider || !pSrcCollider)
		return;

	if (_Dst == _Src)
		return;

	float fWidth(0.f), fHeight(0.f);
	if (Check_Mask(pDstCollider, pSrcCollider)
		&& Check_Rect(pDstCollider, pSrcCollider, &fWidth, &fHeight))
	{
		auto srcOwner = pSrcCollider->GetOwner();
		Vec2 vSrcOwnerPos = srcOwner->GetPos();
		float fSpeed = srcOwner->GetSpeed();

		if (pSrcCollider->Get_Layer() == COL_TOWER)
		{
			pSrcCollider->GetOwner()->SetPosY(vSrcOwnerPos.y);
			return;
		}

		//상하충돌
		if (fWidth > fHeight)
		{
			if (pDstCollider->GetOwner()->GetPos().y < pSrcCollider->GetOwner()->GetPos().y)
			{
				pSrcCollider->GetOwner()->SetPosY(vSrcOwnerPos.y + fHeight * 0.3f);
			}
			else
			{
				pSrcCollider->GetOwner()->SetPosY(vSrcOwnerPos.y - fHeight * 0.3f);
			}
		}
		else
		{
			if (pDstCollider->GetOwner()->GetPos().x < pSrcCollider->GetOwner()->GetPos().x)
			{
				pSrcCollider->GetOwner()->SetPosX(vSrcOwnerPos.x + fWidth * 0.3f);
			}
			else
			{
				pSrcCollider->GetOwner()->SetPosX(vSrcOwnerPos.x - fWidth * 0.3f);
			}
		}
	}
}

bool CCollisionManager::Check_Rect(CCollider* _Dst, CCollider* _Src, float* _pX, float* _pY)
{
	Vec2 vDstPos = _Dst->GetFinalPos();
	Vec2 vDstScale = _Dst->GetScale();

	Vec2 vSrcPos = _Src->GetFinalPos();
	Vec2 vSrcScale = _Src->GetScale();

	//AABB
	float fRadiusX = (vDstScale.x + vSrcScale.x) * 0.5f;
	float fRadiusY = (vDstScale.y + vSrcScale.y) * 0.5f;

	float fWidth = abs(vDstPos.x - vSrcPos.x);
	float fHeight = abs(vDstPos.y - vSrcPos.y);

	if (fRadiusX > fWidth && fRadiusY > fHeight)
	{
		*_pX = fRadiusX - fWidth + 1.f;
		*_pY = fRadiusY - fHeight + 1.f;
		return true;
	}

	return false;
}

bool CCollisionManager::Check_Mask(CCollider* Dst, CCollider* Src)
{
	return ((Dst->Get_Layer() & Src->Get_Mask()) != 0 && (Src->Get_Layer() & Dst->Get_Mask()) != 0);
}
