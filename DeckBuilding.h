#pragma once
#include <cstdlib>
#include <vector>
#include <string>
#include <time.h>
#include <Windows.h>
#include "Card.h"
#include "Stats.h"
#include "macroNum.h"

using namespace std;

#define BSIZE 25
double LenghtPts(int x1, int y1, int x2, int y2);
BOOL InCircle(int x, int y, int mx, int my);

class DeckBuilding
{
private:
	//현재 덱
	vector<Card> myDeck;
	//보유한 카드들
	vector<Card> inven;
	//덱빌딩 할때 성제한 여부 확인하기

public:
	DeckBuilding()
	{
		//기본사이즈 세팅 후 좌표 초기화
		//myDeck.resize(10);
		//for (int i = 0; i < 10; i++)
		//{
		//	int x = i % 5, y = i / 5;		//한 화면에 가로5개, 세로5개
		//	myDeck[i].x = x * 150 + 50, myDeck[i].y = y * 150 + 50; //(val * 간격 + 젤 (왼/위)쪽으로부터의 여백)
		//}
	}

	virtual ~DeckBuilding()
	{

	}

	//인벤토리의 사이즈 리턴
	int GetSize();
	void PushCard(vector<Card> p_cards)
	{
		for (int i = 0; i < p_cards.size(); i++)
		{
			inven.push_back(p_cards[i]);
		}
	}

	//인벤->덱 (덱, 마우스X, 마우스Y)
	void ItoD(int p_mx, int p_my);
	//덱->인벤 (덱, 마우스X, 마우스Y)
	void DtoI(int p_mx, int p_my);
	void DeckBuild(int p_mx, int p_my);

	//인벤토리 출력
	void DrawInventory(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[]);
	//마이덱 출력
	void DrawMyDeck(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[]);
	//덱빌딩 화면 출력
	void DrawDeckBuild(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[]);
};
