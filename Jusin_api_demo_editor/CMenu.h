#pragma once
#include "CScene.h"
class CMenu 
	: public CScene
{
public:
	CMenu();
	virtual ~CMenu();

public:
	void Update() override;
	void Render(HDC _dc) override;
	void Enter() override;
	void Exit() override;

private:
	void Initialize();

	void Render_Map(HDC hdc, int iScrollX, int iScrollY);

private:
	FRAME m_tFrame;
};

