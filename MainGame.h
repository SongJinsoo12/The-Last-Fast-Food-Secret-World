#pragma once
enum State {
	E_START,
	E_MENU,
	E_SHOP,
	E_PVP,
	E_STAGE_CLEAR,
	E_DEAD,
	E_GAMEOVER
};

class MainGame
{
private:
	int Coin;
	int CardBox;
	int LargeStage;
	int SmallStage;

public:
	int GameState = E_START;
	int mx, my;	// 마우스 커서

	MainGame();
	void AddGold(int v);
	void RemoveGold(int v);

	int GetStage();
};

