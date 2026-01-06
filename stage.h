#pragma once
#include "item.h"
#include "Card.h"
#include "MainGame.h"
using namespace std;

class Stage : MainGame
{
protected:
	int L_Stage; //월드
	int S_Stage; //스테이지
	MainGame* Game;
public:
	bool IsLastBossStage();
	bool GameStart();
	int StageClear(bool isMonsterLost);
	bool GameOver(bool isPlayerLost);
	//bool CheckTutorial();
	bool StageStart();
	bool IsGameClear();
	int NextStage();
	
	int Get_LargeStage();
	int Get_SmallStage();
	//int getstage();
	int setgold();
	

};


class TutorialStage : Stage
{
private:
	bool IsTutorial;

public:
	//CType TutorialCard();
	bool CheckTutorial();
};
