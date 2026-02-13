#pragma once
#include "Singleton.h"
#include "ButtonManager.h"

class GameMenu : public Singleton<GameMenu>
{
	friend class Singleton<GameMenu>;
public:
	ButtonManager ButtonMenu;

public:
	void Enter();
	void Logic();
	void Exit();
};

