#include "MainGame.h"

MainGame::MainGame() {
	Coin = 0;
	LargeStage = 1;
	SmallStage = 1;
	CardBox = 0;
}

void MainGame::AddGold(int v) {
	Coin += v;
}

void MainGame::RemoveGold(int v) {
	Coin -= v;
	if (Coin <= 0)
		Coin = 0;
}

int MainGame::GetStage() {
	return LargeStage, SmallStage;
}
