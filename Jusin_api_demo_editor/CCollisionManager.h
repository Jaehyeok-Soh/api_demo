#pragma once
#include "CCollider.h"

class CCollisionManager
{
public:
	static void Collision_Rect_Resolve(CComponent* _Dst, CComponent* _Src);
	static bool Check_Rect(CCollider* _Dst, CCollider* _Src, float* _pX, float* _pY);

	static bool Check_Mask(CCollider* Dst, CCollider* Src);
};

