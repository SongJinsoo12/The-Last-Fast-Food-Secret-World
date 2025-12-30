#pragma once
#include "Card.h"
#include "macroNum.h"

class Stats
{
protected:
	const int generalMaxHealth = 100;

	int health;
	int turn;

	const int maxCard = 26;
	Card deck[DECKMAXSIZE];
	Card card[26];
	
public:
	bool isTurn = false;

	Stats();
	int GetHP();
};

template <typename T>
class Player :public Stats {

};
