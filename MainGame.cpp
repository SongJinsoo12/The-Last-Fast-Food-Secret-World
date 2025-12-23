#include "MainGame.h"

MainGame::MainGame() {
	gold = 0;
	stage = 0;
}

void MainGame::AddGold(int v) {
	gold += v;
}

void MainGame::RemoveGold(int v) {
	gold -= v;
	if (gold <= 0)
		gold = 0;
}

int MainGame::GetStage() {
	return stage;
}