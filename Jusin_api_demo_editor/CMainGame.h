#pragma once
class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();

private:
	void		Load_Img();
	void		Load_PlayerImg();

private:
	HDC m_DC;
};