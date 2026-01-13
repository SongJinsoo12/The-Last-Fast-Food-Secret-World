#pragma once
#include "Card.h"
#include "MainGame.h"
using namespace std;

enum StageState
{
	E_STAGE_TUTORIAL,
	E_STAGE_NORMAL,
	E_STAGE_BOSS,
	E_STAGE_LASTBOSS,
	E_STAGE_GAMECLEAR
};

class Stage : MainGame
{
protected:
	//int L_Stage; //월드
	//int S_Stage; //스테이지
	
public:
	int StageState;
	bool IsBossStage();
	bool IsLastBossStage();

	bool GameStart();
	bool GameOver(bool isPlayerLost);

	//bool CheckTutorial();
	bool StageStart();
	int StageClear(bool isMonsterLost);

	bool IsGameClear();
	int NextStage();
	
	int Get_LargeStage();
	int Get_SmallStage();
	//int getstage();
	int setgold();
	

};

enum TCard
{
	E_TUTORIAL_ATTACK,
	E_TUTORIAL_DEFENSE,
	E_TUTORIAL_MAGIC
};

class TutorialStage : Stage
{
private:
	bool IsTutorial;

public:
	 
	TCard TutorialCard();
	bool CheckTutorial();
};
