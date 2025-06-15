#pragma once
#include "CScene.h"

class CPlay
	: public CScene
{
public:
	CPlay();
	~CPlay();

public:
	void Update() override;
	void Render(HDC _dc) override;
	void Enter() override;
	void Exit() override;

private:
	void Initialize();
	void Key_Input();

	void Render_Map(HDC hdc, int iScrollX, int iScrollY);

private:
	//FRAME m_tFrame;

	float m_fdtPlayTime;
};

