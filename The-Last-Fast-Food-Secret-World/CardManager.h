#pragma once
#include <Windows.h>
#include <vector>

using namespace std;
class GameCard;

#define LOAD 404

//덱 및 패 출력
class CardManager
{
public:
	CardManager();
	void StartGame();
	~CardManager();

	int GetDeckCount();
	int GetHandCount();
	vector<GameCard*> GetHand();
	void SetDeck();
	void CardDraw(int drawNum);
	void CardAct(CardManager& opponent, HWND hWnd);

	void DrawLine(HDC hdc, int startX, int startY, int lengthX, int lengthY);
	void DrawBG();
	void DrawPlayerHand();
	void DrawOppHand();
	void HandSelect(WPARAM wParam, CardManager& opponent, HWND hWnd);
	void StartTurn(CardManager& player, CardManager& opponent);
	void TimeLimit(WPARAM wParam, CardManager& opponent);
	void OpponentAct();

	void SetImage();

private:
	int deckCount;//덱 장수
	int handCount;//패 장수
	int handSelection;//패 카드 선택
	vector<GameCard*> hand;//패 카드
	vector<GameCard*> deck;//덱 카드
	bool isMyTurn;//턴 확인
	bool isSelect;//패 카드 선택 확인
};