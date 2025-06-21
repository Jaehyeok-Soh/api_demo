#pragma once
#include "CScene.h"
#include "DTOConnectInfo.h"

/// <summary>
/// ������ 2����� ���
/// �� Ŭ��� ������ �÷��̾� ��ü�� �����޴´�
/// Ÿ �������� �÷��̾� ������ �����Ѵ�
/// </summary>
class CLoading
	: public CScene
{
public:
	CLoading();
	~CLoading();

public:
	void Update() override;
	void Render(HDC _dc) override;
	void Enter() override;
	void Exit() override;

	void AddPlayer(DTOConnectInfo _player, bool _isMine);

private:
	void Initialize();

	void Render_Map(HDC _dc, int iScrollX, int iScrollY);
};

