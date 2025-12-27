#pragma once
class MainGame
{
private:
	int Coin;
	int CardBox;
	int LargeStage;
	int SmallStage;

public:
	int mx, my;	// 마우스 커서

	MainGame();
	void AddGold(int v);
	void RemoveGold(int v);

	int GetStage();
};

