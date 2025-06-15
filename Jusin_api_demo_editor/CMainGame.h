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

private:
	HDC m_DC;
};