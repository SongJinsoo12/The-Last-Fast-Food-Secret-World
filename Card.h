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

class Card
{
protected:
	int Atk;
	int Rdc;
	CAttribute Ait;
	CType Type;

public:
	Card();
	//카드 위치
	int x;
	int y;
	////드래그
	//bool drag;
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

