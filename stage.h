#pragma once
#include "item.h"
#include "Card.h"
#include "MainGame.h"
using namespace std;

class Stage : MainGame
{
protected:
	int L_stage; //월드
	int S_stage; //스테이지
	MainGame* Game;
public:
	bool IsBossStage(const MainGame& game);
	bool GameStart();
	bool StageClear(bool isMonsterDead);
	bool GameOver(bool isPlayerDead);
	//bool CheckTutorial();
	bool StageStart();
	bool GameClear();
	//int GL_Stage(MainGame& game);
	//int GS_Stage(MainGame& game);

	//int getstage();
	int setgold();
	

};


class TutorialStage : Stage //다중 상속?
{
private:
	bool IsTutorial;

public:
	CType TutorialCard();
	bool CheckTutorial();
};
