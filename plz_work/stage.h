#pragma once
#include <iostream>
#include "MainGame.h"
#include "InputGame.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "ButtonManager.h"

using namespace std;

enum StageState
{
	E_STAGE_TUTORIAL,
	E_STAGE_NORMAL,
	E_STAGE_BOSS,
	E_STAGE_LASTBOSS,
	E_STAGE_GAMECLEAR
};

enum StageName
{
	//hmmmmm
};


class Stage : MainGame
{
protected:


public:
	int StageState; //스테이지 상태
	bool IsBossStage(); //보스 스테이지 확인
	bool IsLastBossStage(); //막보 스테이지 확인

	//bool GameStart(); //게임 시작
	bool GameOver(bool isPlayerLost); //게임 오버

	bool StageStart(); //스테이지 시작
	int StageClear(bool isMonsterLost); //스테이지 클리어

	bool IsGameClear(); //게임 클리어 확인
	int NextStage(); //다음 스테이지 

	int Get_LargeStage(); //튜토리얼 클래스용
	int Get_SmallStage(); //튜토리얼 클래스용
	int DropReward(); //스테이지에 따라 골드 설정
	
		

};

enum TCardType
{
	E_TUTORIAL_ATTACK,
	E_TUTORIAL_DEFENSE,
	E_TUTORIAL_MAGIC
};

class TutorialStage : public Stage
{
private:
	bool IsTutorial;

public:

	TCardType TutorialCard(); //튜토리얼 스테이지에 맞는 카드 반환
	bool CheckTutorial(); //튜토리얼 스테이지 확인
};

//extern TutorialStage g_TutorialStage;

class StageClear : public Stage
{
public:
	ButtonManager m_StageClearBtn;
public:
	StageClear()
	{
		RENDER.SetImage(L"InGameResultBG.png", "StageClear",
			Rect(0, 0, 1280, 720), Rect(0, 0, 0, 0), false, GameImage_M::LayerType::Background);
		RENDER.SetImage(L"StageClearBtn1.png", "StageClearBtn_1", //100x50
			Rect(0, 0, 100, 50), Rect(0, 0, 0, 0), false, GameImage_M::LayerType::UI);
	}
	void DrawStageClearButton();
	void DrawInGameResult();
	void ExitInGameResult();
};

extern StageClear g_StageResult;