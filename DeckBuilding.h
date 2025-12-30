#pragma once
#include <cstdlib>
#include <vector>
#include <string>
#include <time.h>
#include <Windows.h>
#include "Card.h"

using namespace std;
BOOL InCircle(int x, int y, int mx, int my);

class DeckBuilding
{
protected://퍼블릭 과연 괜찮은가
	//현재 덱
	vector<Card> myDeck;
	//보유한 카드들
	vector<Card> inven;

	//임시 전체카드풀/////////////////////
	Card* allcard = new Card[256];

public:
	DeckBuilding()
	{
		//기본사이즈 세팅 후 좌표 초기화
		myDeck.resize(10);
		for (int i = 0; i < 10; i++)
		{
			int x = i % 5, y = i / 5;		//한 화면에 가로5개, 세로5개
			myDeck[i].SetCard(CType(rand() % 3), x * 150 + 50, y * 150 + 50, i);	//(val * 간격 + 젤 (왼/위)쪽으로부터의 여백)
		}

		for (int i = 0; i < 256; i++)
		{
			allcard[i].SetCard(CType(rand() % 3), 0, 0, i);
		}
	}

	virtual ~DeckBuilding()
	{
		//포인터해제 타이밍에 문제있는듯 드로우덱종료시 에러
			//delete[] allcard;
	}

	int GetSize();
	void GetCard(Card* p_cards, int p_size)
	{
		for (int i = 0; i < p_size; i++)
		{
			inven.push_back(p_cards[i]);
		}
	}
	Card* GetAllCard()
	{
		return allcard;
	}

	//인벤->덱 (덱, 마우스X, 마우스Y)
	void ItoD(DeckBuilding& p_deck, int p_mx, int p_my);
	//덱->인벤 (덱, 마우스X, 마우스Y)
	void DtoI(DeckBuilding& p_deck, int p_mx, int p_my);
	void DeckBuild(DeckBuilding& p_deck, int p_mx, int p_my);

	//인벤토리 출력
	void DrawInventory(HDC p_hdc, DeckBuilding p_deck, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[]);
	//마이덱 출력
	void DrawMyDeck(HDC p_hdc, DeckBuilding p_deck, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[]);
	//덱빌딩 화면 출력
	void DrawDeckBuild(HDC p_hdc, DeckBuilding p_deck, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[]);
};