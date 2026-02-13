#pragma once
#include "Singleton.h"


class GameLobby : public Singleton<GameLobby>
{
	friend class Singleton<GameLobby>;

public:
	void Enter();
	void Logic();
	void Exit();
};