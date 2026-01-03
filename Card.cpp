#include "Card.h"

Card::Card() {
	Atk = 0;
	Rdc = 0;
	Ait = E_BREAD;
	Type = E_Attack;
	x = 0;
	y = 0;
}

Deck::Deck() : deckCount(25), handCount(5) // 패 임시 장수
{
	Card temp;
	for (size_t i = 0; i < handCount; i++)
		hand.push_back(temp);
}

Deck::~Deck()
{
}

int Deck::GetDeckCount()
{
	return deckCount;
}

int Deck::GetHandCount()
{
	return handCount;
}

vector<Card> Deck::GetHand()
{
	return hand;
}

//드로우
void Deck::CardDraw()
{
	if (deckCount <= 0)
		return;

	deckCount--;
	handCount++;

	Card temp; //임시
	hand.push_back(temp);
}

//턴 엔드
bool Deck::TurnEnd()
{
	return true;
}
