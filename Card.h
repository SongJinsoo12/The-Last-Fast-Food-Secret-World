#pragma once
#include <vector>
using namespace std;

enum CAttribute {
	E_BULGOGI,
	E_SOURCE,
	E_CHESSE,
	E_VEGAT,
	E_BREAD
};

enum CType {
	E_Attack,
	E_Deffence,
	E_Magic
};

enum CRank
{
	Star_1 = 1,
	Star_2,
	Star_3,
};

class Card
{
protected:
	int Atk;
	int Rdc;
	CAttribute Ait;
	CType Type;
	CRank Rank;//

public:
	Card();
	//카드 위치
	int x;
	int y;
	int id;
	////드래그
	//bool drag;

	////////////////////////////
	void SetCard(CType p_type, int p_id, int p_x, int p_y)
	{
		Type = p_type, x = p_x, y = p_y, id = p_id;
	}
};

//덱 및 패
class Deck
{
public:
	Deck();
	~Deck();

	int GetDeckCount();
	int GetHandCount();
	vector<Card> GetHand();
	void CardDraw();
	bool TurnEnd();

private:
	int deckCount;//덱 장수
	int handCount;//패 장수
	vector<Card> hand; //패 카드
};

