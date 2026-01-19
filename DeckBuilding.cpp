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

vector<Card> DeckBuilding::EraseDuple(vector<Card> p_cards)
{
	//int psize = p_cards.size();
	for (int i = 0; i < p_cards.size(); i++)
	{
		if (p_cards[i].GetUid() < 0)
		{
			p_cards.erase(p_cards.begin() + i);
			for (int j = i; j < p_cards.size(); j++)
			{
				p_cards[j].x -= 75;
				if (p_cards[j].x < 1000)
				{
					p_cards[j].x = 75 * 4 + 1050, p_cards[j].y -= 150;
				}
			}
			--i;
		}
	}
	return p_cards;
}

int DeckBuilding::GetSize()
{
	return this->inven.size();
}

void DeckBuilding::PushCard(vector<Card> p_cards)
{
	//인벤에 들어갈 자리에 맞게 좌표를 세팅함.
	for (int i = 0; i < p_cards.size(); i++)
	{
		int x = (inven.size() + i) % 5, y = (inven.size() + i) / 5;
		p_cards[i].x = x * 75 + 1050, p_cards[i].y = y * 150 + 50;
	}
	//좌표 세팅 후 중복을 제거하고
	p_cards = EraseDuple(p_cards);
	//완성된 배열을 인벤에 추가
	inven.insert(inven.end(), p_cards.begin(), p_cards.end());
}

void DeckBuilding::ItoD(int p_mx, int p_my)
{
	for (int i = 0; i < inven.size(); i++)
	{
		//카드가 클릭되었을경우
		if (InCircle(inven[i].x, inven[i].y, p_mx, p_my))
		{
			Card selectedCard = inven[i];
			//덱이 꽉 차면 종료
			if (myDeck.size() >= DECKMAXSIZE)
			{
				cout << "덱 꽉참" << "\n";
				return;
			}
			//덱에 들어갈 수 있는 1성이 최대라면 종료(레어도 추가해주겠지)
			else if ((selectedCard.GetType() != E_Magic && selectedCard.GetAtk() == 5) && Star_n[0] >= 15)
			{
				cout << "1성 꽉참" << "\n";
				return;
			}
			//덱에 들어갈 수 있는 2성이 최대라면 종료
			else if ((selectedCard.GetType() != E_Magic && selectedCard.GetAtk() == 10) && Star_n[1] >= 7)
			{
				cout << "2성 꽉참" << "\n";
				return;
			}
			//덱에 들어갈 수 있는 3성이 최대라면 종료
			else if ((selectedCard.GetType() != E_Magic && selectedCard.GetAtk() == 15) && Star_n[2] >= 3)
			{
				cout << "3성 꽉참" << "\n";
				return;
			}

			++Star_n[(int)(selectedCard.GetAtk() / 5) - 1];
			//(val * 간격 + 젤(왼 / 위)쪽으로부터의 여백)
			//이동시킬 카드의 좌표를 변경. 출발지 배열에서 제거 후 목적지 배열 맨 뒤에 추가
			int x = myDeck.size() % 5, y = myDeck.size() / 5;
			inven[i].x = x * 120 + 450, inven[i].y = y * 150 + 50;
			myDeck.push_back(inven[i]);
			inven.erase(inven.begin() + i);

			//이동된 카드의 뒤쪽 카드들을 앞으로 한칸씩 좌표를 변경
			for (int j = i; j < inven.size(); j++)
			{
				inven[j].x -= 75;
				if (inven[j].x < 1000)
				{
					inven[j].x = 75 * 4 + 1050, inven[j].y -= 150;
				}
			}
			return;
		}
	}
}

void DeckBuilding::DtoI(int p_mx, int p_my)
{
	for (int i = 0; i < myDeck.size(); i++)
	{
		//카드가 클릭되었을경우
		if (InCircle(myDeck[i].x, myDeck[i].y, p_mx, p_my))
		{
			Card selectedCard = myDeck[i];
			--Star_n[(int)(selectedCard.GetAtk() / 5) - 1];
			//이동시킬 카드의 좌표를 변경. 출발지 배열에서 제거 후 목적지 배열 맨 뒤에 추가
			int x = inven.size() % 5, y = inven.size() / 5;
			myDeck[i].x = x * 75 + 1050, myDeck[i].y = y * 150 + 50;
			inven.push_back(myDeck[i]);
			myDeck.erase(myDeck.begin() + i);

			//이동된 카드의 뒤쪽 카드들을 앞으로 한칸씩 좌표를 변경
			for (int j = i; j < myDeck.size(); j++)
			{
				myDeck[j].x -= 120;
				if (myDeck[j].x < 400)
				{
					myDeck[j].x = 120 * 4 + 450, myDeck[j].y -= 150;
				}
			}
			return;
		}
	}
}

void DeckBuilding::DeckBuild(int p_mx, int p_my)
{
	this->ItoD(p_mx, p_my);	//인벤->마이덱
	this->DtoI(p_mx, p_my);	//마이덱->인벤
}

void DeckBuilding::DrawInventory(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[])
{
	for (int i = 0; i < inven.size(); i++)
	{
		if (InCircle(inven[i].x, inven[i].y, p_mx, p_my))
		{
			p_hpen = CreatePen(PS_SOLID, 5, RGB(255, 255, 0));
			p_oldpen = (HPEN)SelectObject(p_hdc, p_hpen);
		}
		Rectangle(p_hdc, inven[i].x - 15, inven[i].y - 30, inven[i].x + 15, inven[i].y + 30);
		SelectObject(p_hdc, p_oldpen);
		DeleteObject(p_hpen);	//펜생성 / 펜원상복구

		wsprintf(p_text, TEXT("%d"), inven[i].GetUid());
		TextOut(p_hdc, inven[i].x - 10, inven[i].y, p_text, lstrlen(p_text));
		wsprintf(p_text, TEXT("%d"),(int)(inven[i].GetAtk() / 5));
		TextOut(p_hdc, inven[i].x - 10, inven[i].y + 13, p_text, lstrlen(p_text));
	}
}

void DeckBuilding::DrawMyDeck(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[])
{
	for (int i = 0; i < myDeck.size(); i++)
	{
		if (myDeck[i].x <= 0)
		{
			break;
		}
		if (InCircle(myDeck[i].x, myDeck[i].y, p_mx, p_my))
		{
			p_hpen = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
			p_oldpen = (HPEN)SelectObject(p_hdc, p_hpen);
		}
		Rectangle(p_hdc, myDeck[i].x - 15, myDeck[i].y - 30, myDeck[i].x + 15, myDeck[i].y + 30);
		SelectObject(p_hdc, p_oldpen);
		DeleteObject(p_hpen);	//펜생성 / 펜원상복구

		wsprintf(p_text, TEXT("%d"), myDeck[i].GetUid());
		TextOut(p_hdc, myDeck[i].x - 10, myDeck[i].y, p_text, lstrlen(p_text));
		wsprintf(p_text, TEXT("%d"), (int)(myDeck[i].GetAtk() / 5));
		TextOut(p_hdc, myDeck[i].x - 10, myDeck[i].y + 13, p_text, lstrlen(p_text));
	}
}

void DeckBuilding::DrawDeckBuild(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[])
{
	DrawInventory(p_hdc, p_hpen, p_oldpen, p_mx, p_my, p_text);	//인벤
	DrawMyDeck(p_hdc, p_hpen, p_oldpen, p_mx, p_my, p_text);	//마이덱

	//카드설명화면
	MoveToEx(p_hdc, 400, 0, NULL);
	LineTo(p_hdc, 400, 720);
	//마이덱
	MoveToEx(p_hdc, 1000, 0, NULL);
	LineTo(p_hdc, 1000, 720);
	//인벤토리
}
