#pragma once

#include <memory>

class MainGame
{
private:
	int Coin;
	int CardBox;
	int LargeStage;
	int SmallStage;
	std::unique_ptr<State> gameState = nullptr;

public:
	int mx, my;	// 마우스 커서

	MainGame();
	void AddGold(int v);
	void RemoveGold(int v);

	int GetStage();
	void StateChange(State v);
};

//class State {
//public:
//	virtual void Update();
//};
//
//class Game_Start :public State {
//	void Update() override;
//};
