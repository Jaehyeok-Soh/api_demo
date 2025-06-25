#pragma once
#include "CScene.h"

class CWaitRoom
	: public CScene
{
public:
	CWaitRoom();
	~CWaitRoom();

public:
	void Initialize();
	void Update() override;
	void Render(HDC _dc) override;
	void Enter() override;
	void Exit() override;

private:
	void Render_Map(HDC hdc);
};