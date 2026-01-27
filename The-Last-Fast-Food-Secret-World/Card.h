#pragma once

#include <random>
#include <iostream>

using namespace std;

#define TURNTIME 777
#define CARDX 100
#define CARDY 132
#define randomInit(start, end)  \
	random_device rd; \
	mt19937 gen(rd()); \
	uniform_int_distribution<int> cookRandom(start, end)

enum CAttribute {
	E_BULGOGI,
	E_SOURCE,
	E_CHESSE,
	E_VEGAT,
	E_BREAD
};

enum CType {
	E_Attack,
	E_Deffense,
	E_Magic
};

enum ALLCARDEnum
{
	BASEATK = 0,
	ATK01,
	ATK02,
	ATKLIMIT = 41,

	BASEDEF = 100,
	DEF01,
	DEF02,
	DEFLIMIT = 144,

	BASEMAGIC = 200,
	MAGIC01,
	MAGIC02,
	MAGICLIMIT = 233,
};

enum Star
{
	E_ONE = 0,
	E_TWO,
	E_THREE,
};

class Card
{
protected:
	int uid;
	int atk;
	int def;
	CAttribute Ait;
	CType Type;
	Star star;

public:
	Card();
	Card(int p_uid);
	void Init();

	//Get Set ÇÔ¼ö
	int GetUid();
	void SetUid(int p_uid);
	int GetAtk();
	void SetAtk(int p_atk);
	int GetDef();
	void SetDef(int p_def);
	CAttribute GetAit();
	void SetAit(CAttribute p_Ait);
	CType GetType();
	void SetType(CType p_Type);
	Star GetStar();
	void SetStar(Star p_Star);

};

class GameCard : public Card
{
public:
	GameCard();
	GameCard(Card* p_Card);
	virtual ~GameCard();

private:

};


