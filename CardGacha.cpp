#include "CardGacha.h"

void CardGacha::one(DeckBuilding& p_deck)
{
	int invensize = p_deck.GetSize();
	Card* all = p_deck.GetAllCard();
	Card out = all[rand() % AllCARDMAXSIZE];
	int x = invensize % 5, y = invensize / 5;
	out.x = x * 150 + 750, out.y = y * 150 + 50;
	p_deck.PushCard(&out, 1);
	return;
}

void CardGacha::ten(DeckBuilding& p_deck)
{
	Card out[10];
	int invensize = p_deck.GetSize();
	Card* all = p_deck.GetAllCard();
	for (int i = 0; i < 10; i++)
	{
		out[i] = all[rand() % AllCARDMAXSIZE];
		int x = (invensize + i) % 5, y = (invensize + i) / 5;
		out[i].x = x * 150 + 750, out[i].y = y * 150 + 50;
	}
	p_deck.PushCard(out, 10);
}

//뽑기를 함(TRUE-1뽑 / FALSE-10뽑, 뽑은카드를 저장할 변수)
void CardGacha::GetGacha(bool isOne, DeckBuilding& p_deck)
{
	//분기나눠서 1차 or 10차 나누기
	if (isOne)
	{
		//RemoveGold(10);
		one(p_deck);
	}
	else if (!isOne)
	{
		ten(p_deck);
	}
}

void CardGacha::DrawGacha(HDC p_hdc, DeckBuilding p_deck, int p_price, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[])
{
	//뽑은카드 출력하게 수정할것///////////////////
	for (int i = 0; i < draw_card.size(); i++)
	{
		//Rectangle(p_hdc, draw_card[i].x - 15, draw_card[i].y - 30, draw_card[i].x + 15, draw_card[i].y + 30);
	}

	//1뽑 버튼
	if (InCircle(850, 635, p_mx, p_my))
	{
		p_hpen = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
		p_oldpen = (HPEN)SelectObject(p_hdc, p_hpen);
	}
	Rectangle(p_hdc, 700, 615, 1000, 685);
	SelectObject(p_hdc, p_oldpen);
	DeleteObject(p_hpen);	//펜생성 / 펜원상복구
	wsprintf(p_text, TEXT("1개 - %dG"), p_price);
	TextOut(p_hdc, 830, 645, p_text, lstrlen(p_text));

	//10뽑 버튼
	if (InCircle(1200, 635, p_mx, p_my))
	{
		p_hpen = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
		p_oldpen = (HPEN)SelectObject(p_hdc, p_hpen);
	}
	Rectangle(p_hdc, 1050, 615, 1350, 685);
	SelectObject(p_hdc, p_oldpen);
	DeleteObject(p_hpen);	//펜생성 / 펜원상복구
	wsprintf(p_text, TEXT("10개 - %dG"), p_price * 9);
	TextOut(p_hdc, 1170, 645, p_text, lstrlen(p_text));
}
