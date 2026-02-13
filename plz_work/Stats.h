#pragma once

template <typename T>
//const int maxCard = 26;
#define maxCard 26
class Stats
{
private:
	const int generalMaxHealth = 100;

	int health;
	int turn;

	T deck[maxCard];
	T card[maxCard];
	
public:
	bool isTurn = false;

	Stats();
	int GetHP();
};

template <typename T>
class Player :public Stats<T> {

};

