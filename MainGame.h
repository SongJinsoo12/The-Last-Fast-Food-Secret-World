#pragma once
class MainGame
{
private:
	int gold;
	int stage;
public:
	MainGame();
	void AddGold(int v);
	void RemoveGold(int v);

	int GetStage();
};

