#pragma once
#include "DeckBuilding.h"
#include "MainGame.h"
#include "macroNum.h"
#include "Shop.h"

class CardGacha : public Shop
{
private:
	vector<Card> draw_card;

public:
	CardGacha()
	{

	}
	virtual ~CardGacha()
	{

	}

	//전체카드중 1장을 인벤에저장 / 사이즈는 클래스내 변수로 저장해두기
	void one(DeckBuilding& p_deck);
	//전체카드 중 10장을 인벤에저장
	void ten(DeckBuilding& p_deck);
	void GetGacha(bool isOne, DeckBuilding& p_deck);

	void DrawGacha(HDC p_hdc, DeckBuilding p_deck, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[]);
};

