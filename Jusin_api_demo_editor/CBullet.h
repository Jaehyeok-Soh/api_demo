#pragma once
#include "CObject.h"
class CBullet
	: public CObject
{
public:
	CBullet();
	~CBullet();

public:
	void Initialize() override;
	void Initialize(CObject* _pTarget);
	int Update() override;
	void Late_Update() override;
	void Render(HDC _dc) override;
	void Release() override;

public:
	//const CObject* GetBulletOwner() const { return pBulletOwner; }
	//void SetBulletOwner(CObject* _pBulletOwner) { pBulletOwner = _pBulletOwner; }

	const CObject* GetBulletWeapon() const { return pBulletWeapon; }
	void SetBulletWeapon(CObject* _pWeapon) { pBulletWeapon = _pWeapon; }

public:
	void OnCollisionEnter(CCollider* _pOther) override;
	void OnCollision(CCollider* _pOther) override;


private:
	void CalcAngle();
	void MoveToAngle();

private:
	float m_fAngle;
	int m_iDamage;
	//CObject* pBulletOwner;
	CObject* pBulletWeapon;
};