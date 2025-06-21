#pragma once
#include "CScene.h"
#include "DTOConnectInfo.h"

/// <summary>
/// 접속자 2명까지 대기
/// 각 클라는 본인의 플레이어 객체를 지정받는다
/// 타 접속자의 플레이어 정보도 지정한다
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

