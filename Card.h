#pragma once
#include <vector>
#include <Windows.h>
#include <random>
using namespace std;

#define TURNTIME 777
#define randomInit(start, end)  \
	random_device rd; \
	mt19937 gen(rd()); \
	uniform_int_distribution<int> cookRandom(start, end)

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
	void CardAct(CardManager& opponent, HWND hWnd);

	void DrawLine(HDC hdc, int startX, int startY, int lengthX, int lengthY);
	void DrawBG(HDC hdc, RECT rect, int cardX, int cardY);
	void DrawDeckCount(HDC hdc,  int rtX, int rtY, int cardX, int cardY);
	void DrawHand(HDC hdc, int rtX, int rtY, int cardX, int cardY, bool isPlayer);
	void HandSelect(WPARAM wParam, CardManager& opponent, HWND hWnd);
	void StartTurn(CardManager& player, CardManager& opponent);
	void TimeLimit(WPARAM wParam, CardManager& opponent, HWND hWnd, LPCWSTR text, LPCWSTR caption, UINT type);

private:
	unsigned int deckCount;//덱 장수
	unsigned int handCount;//패 장수
	int handSelection;//패 카드 선택
	vector<Card> hand;//패 카드
	bool isMyTurn;//턴 확인
};
