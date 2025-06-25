#pragma once
#include "CScene.h"

class CLobby
	: public CScene
{
public:
	CLobby();
	~CLobby();

public:
	void Initialize() override;
	void Update() override;
	void Render(HDC _dc) override;
	void Enter() override;
	void Exit() override;

private:
	void Render_Map(HDC hdc);
};

