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

public:
	void ClientCursorLock();
	void ClientCursorUnlock();

private:
	void		Load_Img();
	void		Load_CharacterImg();
	void		Load_TowerImg();

private:
	HDC m_DC;
	HDC m_hBackDC = nullptr;
	HBITMAP m_hBackBmp = nullptr;
	HBITMAP m_hOldBmp = nullptr;
};