#pragma once
#include "CScene.h"
#include "CTile.h"
class CEdit
	: public CScene
{
public:
	
public:
	CEdit();
	virtual ~CEdit();

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
	FRAME m_tFrame;

	TILETYPE TileSetDrawID;
	int TileSetOption;
};

