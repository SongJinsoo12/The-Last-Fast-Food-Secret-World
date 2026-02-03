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
	int GetGold()
	{
		return this->Coin;
	}
	void AddGold(int v);
	bool RemoveGold(int v);

	int GetStage();
};

<<<<<<< HEAD
extern MainGame g_MainGame;

=======
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
