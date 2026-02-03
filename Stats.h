#pragma once

template <typename T>
class Stats
{
private:
	const int generalMaxHealth = 100;

	int health;
	int turn;

	const int maxCard = 26;
	T deck[26];
	T card[26];
	
public:
	bool isTurn = false;

	Stats();
	int GetHP();
};

template <typename T>
class Player :public Stats<T> {

};

