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

enum class CardId : int
{
	Cockroach = 9999
};

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
	int m_Uid;
	int m_Atk;
	int m_Def;
	CAttribute m_Ait;
	CType m_Type;
	Star m_Star;
	string m_info;
public:
	int x, y;

public:
	Card();
	Card(int p_uid);
	void Init();

	//Get Set 함수
	int GetUid() const;
	void SetUid(int p_uid);
	int GetAtk() const;
	void SetAtk(int p_atk);
	int GetDef() const;
	void SetDef(int p_def);
	CAttribute GetAit() const;
	void SetAit(CAttribute p_Ait);
	CType GetType() const;
	void SetType(CType p_Type);
	Star GetStar() const;
	void SetStar(Star p_Star);
	/*int GetUid();
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
	void SetStar(Star p_Star);*/
	string GetInfo();
	void SetInfo(string p_info);

	// 실제 공격카드(AtkCard)에 배율을 적용하고,
	// 적용 후 m_hasNextAtkMultiplier를 false로 끄는 식으로 "1회성" 처리하는 용도
	void setAttribute(CAttribute attr) { m_Ait = attr; } // 추가
};

class GameCard : public Card
{
public:
	GameCard();
	GameCard(Card* p_Card);
	virtual ~GameCard();

private:

};


