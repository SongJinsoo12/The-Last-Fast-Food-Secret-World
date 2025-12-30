#pragma once
#include "item.h"
#include "card.h"
#include "MainGame.h"
using namespace std;

class Stage : Card
{
private:
	int largeStage;
	int smallStage; 
	MainGame* mainGame;
public:
	bool IsBossStage(const MainGame& game) ;
	bool GameStart();
	bool StageClear(bool isMonsterDead, MainGame& game);
	bool GameOver(bool isPlayerDead);
	bool TutorialStart();
	bool StageStart();
	bool GameClear();
	//int getstage();
	int setgold();
	CType TutorialCard();
	void Init(int large, int small);

};
