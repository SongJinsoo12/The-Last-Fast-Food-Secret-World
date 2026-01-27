#include "DeckBuilding.h"

double LenghtPts(int x1, int y1, int x2, int y2)
{
	return sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

BOOL InCircle(int x, int y, int mx, int my)
{
	if (LenghtPts(x, y, mx, my) < BSIZE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int DeckBuilding::GetSize()
{
	return this->inven.size();
}

void DeckBuilding::ItoD(DeckBuilding& p_deck, int p_mx, int p_my)
{
	for (int i = 0; i < p_deck.inven.size(); i++)
	{
		//카드가 클릭되었을경우
		if (InCircle(p_deck.inven[i].x, p_deck.inven[i].y, p_mx, p_my))
		{
			//덱이 꽉 차면 함수종료
			if (p_deck.myDeck.size() >= 25)
			{
				return;
			}

			//이동시킬 카드의 좌표를 변경. 출발지 배열에서 제거 후 목적지 배열 맨 뒤에 추가
			int x = p_deck.myDeck.size() % 5, y = p_deck.myDeck.size() / 5;
			p_deck.inven[i].x = x * 150 + 50, p_deck.inven[i].y = y * 150 + 50;
			p_deck.myDeck.push_back(p_deck.inven[i]);
			p_deck.inven.erase(p_deck.inven.begin() + i);

			//이동된 카드의 뒤쪽 카드들을 앞으로 한칸씩 좌표를 변경
			for (int j = i; j < p_deck.inven.size(); j++)
			{
				p_deck.inven[j].x -= 150;
				if (p_deck.inven[j].x < 700)
				{
					p_deck.inven[j].x = 150 * 4 + 750, p_deck.inven[j].y -= 150;
				}
			}
			return;
		}
	}
}

void DeckBuilding::DtoI(DeckBuilding& p_deck, int p_mx, int p_my)
{
	for (int i = 0; i < p_deck.myDeck.size(); i++)
	{
		//카드가 클릭되었을경우
		if (InCircle(p_deck.myDeck[i].x, p_deck.myDeck[i].y, p_mx, p_my))
		{
			//이동시킬 카드의 좌표를 변경. 출발지 배열에서 제거 후 목적지 배열 맨 뒤에 추가
			int x = p_deck.inven.size() % 5, y = p_deck.inven.size() / 5;
			p_deck.myDeck[i].x = x * 150 + 750, p_deck.myDeck[i].y = y * 150 + 50;
			p_deck.inven.push_back(p_deck.myDeck[i]);
			p_deck.myDeck.erase(p_deck.myDeck.begin() + i);

			//이동된 카드의 뒤쪽 카드들을 앞으로 한칸씩 좌표를 변경
			for (int j = i; j < p_deck.myDeck.size(); j++)
			{
				p_deck.myDeck[j].x -= 150;
				if (p_deck.myDeck[j].x < 0)
				{
					p_deck.myDeck[j].x = 150 * 4 + 50, p_deck.myDeck[j].y -= 150;
				}
			}
			return;
		}
	}
}

void DeckBuilding::DeckBuild(DeckBuilding& p_deck, int p_mx, int p_my)
{
	p_deck.ItoD(p_deck, p_mx, p_my);	//인벤->마이덱
	p_deck.DtoI(p_deck, p_mx, p_my);	//마이덱->인벤
}

void DeckBuilding::DrawInventory(HDC p_hdc, DeckBuilding p_deck, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[])
{
	for (int i = 0; i < p_deck.inven.size(); i++)
	{
		if (InCircle(p_deck.inven[i].x, p_deck.inven[i].y, p_mx, p_my))
		{
			p_hpen = CreatePen(PS_SOLID, 5, RGB(255, 255, 0));
			p_oldpen = (HPEN)SelectObject(p_hdc, p_hpen);
		}
		Rectangle(p_hdc, p_deck.inven[i].x - 15, p_deck.inven[i].y - 30, p_deck.inven[i].x + 15, p_deck.inven[i].y + 30);
		SelectObject(p_hdc, p_oldpen);
		DeleteObject(p_hpen);	//펜생성 / 펜원상복구

		wsprintf(p_text, TEXT("%d"), p_deck.inven[i].id);
		TextOut(p_hdc, p_deck.inven[i].x - 10, p_deck.inven[i].y, p_text, lstrlen(p_text));
	}
}

void DeckBuilding::DrawMyDeck(HDC p_hdc, DeckBuilding p_deck, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[])
{
	for (int i = 0; i < p_deck.myDeck.size(); i++)
	{
		if (p_deck.myDeck[i].x <= 0)
		{
			break;
		}
		if (InCircle(p_deck.myDeck[i].x, p_deck.myDeck[i].y, p_mx, p_my))
		{
			p_hpen = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
			p_oldpen = (HPEN)SelectObject(p_hdc, p_hpen);
		}
		Rectangle(p_hdc, p_deck.myDeck[i].x - 15, p_deck.myDeck[i].y - 30, p_deck.myDeck[i].x + 15, p_deck.myDeck[i].y + 30);
		SelectObject(p_hdc, p_oldpen);
		DeleteObject(p_hpen);	//펜생성 / 펜원상복구

		wsprintf(p_text, TEXT("%d"), p_deck.myDeck[i].id);
		TextOut(p_hdc, p_deck.myDeck[i].x - 10, p_deck.myDeck[i].y, p_text, lstrlen(p_text));
	}
}

void DeckBuilding::DrawDeckBuild(HDC p_hdc, DeckBuilding p_deck, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[])
{
	p_deck.DrawInventory(p_hdc, p_deck, p_hpen, p_oldpen, p_mx, p_my, p_text);	//인벤
	p_deck.DrawMyDeck(p_hdc, p_deck, p_hpen, p_oldpen, p_mx, p_my, p_text);	//마이덱

	//중앙경계선
	MoveToEx(p_hdc, 700, 0, NULL);
	LineTo(p_hdc, 700, 720);
}
