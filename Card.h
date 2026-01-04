#pragma once
#include <vector>
#include <Windows.h>
using namespace std;

enum CAttribute {
	E_BULGOGI,
	E_SOURCE,
	E_CHESSE,
	E_VEGAT,
	E_BREAD
};

enum CType {
	E_Attack,
	E_Deffence,
	E_Magic
};

class Card
{
protected:
	int Atk;
	int Rdc;
	CAttribute Ait;
	CType Type;

public:
	Card();
	//카드 위치
	int x;
	int y;
};

//덱 및 패
class CardManager
{
public:
	CardManager();
	~CardManager();

	int GetDeckCount();
	int GetHandCount();
	vector<Card> GetHand();
	void CardDraw();
	bool TurnEnd();
	void CardAct();

	void DrawLine(HDC hdc, int startX, int startY, int lengthX, int lengthY);
	void DrawBG(HDC hdc, RECT rect, int cardX, int cardY);
	void DrawDeckCount(HDC hdc,  int rtX, int rtY, int cardX, int cardY);
	void DrawHand(HDC hdc, int rtX, int rtY, int cardX, int cardY);
	void HandSelect(WPARAM wParam);

private:
	unsigned int deckCount;//덱 장수
	unsigned int handCount;//패 장수
	int handSelection;//패 카드 선택
	vector<Card> hand; //패 카드
};
