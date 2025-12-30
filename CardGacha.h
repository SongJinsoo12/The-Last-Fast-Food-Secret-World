#pragma once
#include "DeckBuilding.h"
#include "MainGame.h"

class CardGacha
{
private:
	vector<Card> draw_card;

public:
	CardGacha();
	virtual ~CardGacha();

	//전체카드중 1장을 리턴 / 사이즈는 클래스내 변수로 저장해두기
	void One(DeckBuilding& p_deck);
	//전체카드 중 10장을 리턴
	void Ten(DeckBuilding& p_deck);
	void GetGacha(bool isOne, DeckBuilding& p_deck);

	void DrawGacha(HDC p_hdc, DeckBuilding p_deck, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[]);
};

