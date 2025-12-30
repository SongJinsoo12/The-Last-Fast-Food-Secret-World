#include "CardGacha.h"
#include "macroNum.h"

void CardGacha::One(DeckBuilding& p_deck)
{
	int invensize = p_deck.GetSize();
	Card* all = p_deck.GetAllCard();
	Card out = all[rand() % AllCARDMAXSIZE];
	int x = invensize % 5, y = invensize / 5;
	out.x = x * 150 + 750, out.y = y * 150 + 50;
	p_deck.GetCard(&out, 1);
	return;
}

void CardGacha::Ten(DeckBuilding& p_deck)
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
	p_deck.GetCard(out, 10);
}

void CardGacha::GetGacha(bool isOne, DeckBuilding & p_deck)
{
	//분기나눠서 1차 or 10차 나누기
	if (isOne)
	{
		//RemoveGold(10);
		One(p_deck);
	}
	else if (!isOne)
	{
		Ten(p_deck);
	}
}

void CardGacha::DrawGacha(HDC p_hdc, DeckBuilding p_deck, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[])
{
	//뽑은카드 출력하게 수정할것///////////////////
	for (int i = 0; i < draw_card.size(); i++)
	{
		Rectangle(p_hdc, draw_card[i].x - 15, draw_card[i].y - 30, draw_card[i].x + 15, draw_card[i].y + 30);
	}

	if (InCircle(450, 635, p_mx, p_my))
	{
		p_hpen = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
		p_oldpen = (HPEN)SelectObject(p_hdc, p_hpen);
	}
	Rectangle(p_hdc, 300, 600, 600, 670);
	SelectObject(p_hdc, p_oldpen);
	DeleteObject(p_hpen);	//펜생성 / 펜원상복구

	if (InCircle(950, 635, p_mx, p_my))
	{
		p_hpen = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
		p_oldpen = (HPEN)SelectObject(p_hdc, p_hpen);
	}
	Rectangle(p_hdc, 800, 600, 1100, 670);
	SelectObject(p_hdc, p_oldpen);
	DeleteObject(p_hpen);	//펜생성 / 펜원상복구

	TextOut(p_hdc, 450, 635, TEXT("1"), 1);
	TextOut(p_hdc, 950, 635, TEXT("10"), 2);
}