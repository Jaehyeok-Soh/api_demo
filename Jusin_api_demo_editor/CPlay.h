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

public:
	void GameSet(bool isWin) { gameSet = true; win = isWin; }

private:
	void Initialize();
	void Key_Input();

	void Render_Map(HDC hdc, int iScrollX, int iScrollY);
	void Render_UI(HDC hdc);
	void Render_Eog(HDC hdc);
	void Render_Eog_Base(HDC hdc);
	void Render_Eog_Title(HDC hdc);
	void Render_Eog_Color(HDC hdc);

	void Update_Frame();
	void Update_Eog_Frame();
	void Update_Eog_Color_Frame();
private:
	bool gameSet;
	int win;

	bool setEogButton;

	FRAME m_tEndingFrame;
	FRAME m_tEogColorFrame;
	HDC m_EogDC;
};

