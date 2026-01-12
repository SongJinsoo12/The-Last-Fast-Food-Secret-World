#include "CardGacha.h"

void CardGacha::one(DeckBuilding& p_deck)
{
	int invensize = p_deck.GetSize();
	int randID = allCard[rand() % AllCARDMAXSIZE]->GetUid();
	Card out;
	out.SetUid(randID);
	int x = invensize % 5, y = invensize / 5;
	out.x = x * 150 + 750, out.y = y * 150 + 50;
	p_deck.PushCard(&out, 1);

	draw_card.resize(1);
	draw_card[0] = out;
}

void CardGacha::ten(DeckBuilding& p_deck)
{
	Card out[10];
	int invensize = p_deck.GetSize();
	for (int i = 0; i < 10; i++)
	{
		int randID = allCard[rand() % AllCARDMAXSIZE]->GetUid();
		out[i].SetUid(randID);
		int x = (invensize + i) % 5, y = (invensize + i) / 5;
		out[i].x = x * 150 + 750, out[i].y = y * 150 + 50;
	}
	p_deck.PushCard(out, 10);

	draw_card.resize(10);
	for (int i = 0; i < 10; i++)
	{
		draw_card[i] = out[i];
	}
}

//뽑기를 함(TRUE-1뽑 / FALSE-10뽑, 뽑은카드를 저장할 변수)
void CardGacha::GetGacha(bool isOne, DeckBuilding& p_deck, MainGame& p_mg, Chest p_selChest)
{
	int remove_gold = p_selChest.GetPrice();
	//분기나눠서 1차 or 10차 나누기
	if (isOne)
	{
		if (!p_mg.RemoveGold(remove_gold))
		{
			this->isGachaFailed = true;
		}
		one(p_deck);
	}
	else if (!isOne)
	{
		if (!p_mg.RemoveGold(remove_gold * 9))
		{
			this->isGachaFailed = true;
		}
		ten(p_deck);
	}
}

void CardGacha::InGacha()
{
	int size = draw_card.size();

	if (size == 1)
	{
		draw_card[0].x = 700, draw_card[0].y = 350;
	}
	else if (size == 10)
	{
		for (int i = 0; i < size; i++)
		{
			draw_card[i].x = (i % 5) * 250 + 200, draw_card[i].y = (i / 5) * 250 + 200;
		}
	}
}

void CardGacha::DrawGachaButton(HDC p_hdc, DeckBuilding p_deck, Chest p_selChest, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[])
{
	//1뽑 버튼
	if (InCircle(850, 635, p_mx, p_my))
	{
		p_hpen = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
		p_oldpen = (HPEN)SelectObject(p_hdc, p_hpen);
	}
	Rectangle(p_hdc, 700, 615, 1000, 685);
	SelectObject(p_hdc, p_oldpen);
	DeleteObject(p_hpen);	//펜생성 / 펜원상복구
	wsprintf(p_text, TEXT("1개 - %dG"), p_selChest.GetPrice());
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
	wsprintf(p_text, TEXT("10개 - %dG"), p_selChest.GetPrice() * 9);
	TextOut(p_hdc, 1170, 645, p_text, lstrlen(p_text));

	if (this->isGachaFailed)
	{
		TextOut(p_hdc, 50, 500, TEXT("돈이 부족합니다."), 10);
	}
}

void CardGacha::DrawGacha(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[])
{
	//뽑은카드 출력하게 수정할것///////////////////
	for (int i = 0; i < draw_card.size(); i++)
	{
		Rectangle(p_hdc, draw_card[i].x - 45, draw_card[i].y - 75, draw_card[i].x + 45, draw_card[i].y + 75);
		wsprintf(p_text, TEXT("%d"), draw_card[i].GetUid());
		TextOut(p_hdc, draw_card[i].x - 2, draw_card[i].y, p_text, lstrlen(p_text));
	}

	wsprintf(p_text, TEXT("왼쪽상단의 상점 버튼을 눌러 돌아가기"));
	TextOut(p_hdc, 690, 650, p_text, lstrlen(p_text));
}
