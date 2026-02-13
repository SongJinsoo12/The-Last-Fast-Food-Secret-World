#include "MainGame.h"

MainGame g_MainGame;

MainGame::MainGame() {
	Coin = 0;
	LargeStage = 1;
	SmallStage = 1;
	CardBox = 0;
}

void MainGame::AddGold(int v) {
	Coin += v;
}

bool MainGame::RemoveGold(int v) {
	int result = Coin - v;
	if (result < 0)
	{
		return false;
	}
	else
	{
		Coin -= v;
		return true;
	}
}


int MainGame::GetLargeStage() {
	return LargeStage;
}

int MainGame::GetSmallStage() {
	return SmallStage;
}