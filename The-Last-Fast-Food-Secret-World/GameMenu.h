#pragma once
#include "Singleton.h"

class GameMenu : public Singleton<GameMenu>
{
	friend class Singleton<GameMenu>;

public:
	void Enter();
	void Logic();
	void Exit();
};

