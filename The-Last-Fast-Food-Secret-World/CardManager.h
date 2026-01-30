#pragma once
#include <Windows.h>
#include <vector>
#include "BossAI.h"

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
	void CardAct(CardManager& player, CardManager& opponent, HWND hWnd);

	void DrawLine(HDC hdc, int startX, int startY, int lengthX, int lengthY);
	void DrawBG();
	void DrawPlayerHand();
	void DrawOppHand();
	void HandSelect(WPARAM wParam, CardManager& player, CardManager& opponent, HWND hWnd);
	void StartTurn(CardManager& player, CardManager& opponent, HWND hWnd);
	void TimeLimit(WPARAM wParam, HWND hWnd, CardManager& player, CardManager& opponent);
	void OpponentAct(CardManager& player, CardManager& opponent, HWND hWnd);

	void SetImage();

private:
	int m_DeckCount;//덱 장수
	int m_HandCount;//패 장수
	int m_HandSelection;//패 카드 선택
	vector<GameCard*> m_Hand;//패 카드
	vector<GameCard*> m_Deck;//덱 카드
	bool m_IsMyTurn;//턴 확인
	bool m_IsSelect;//패 카드 선택 확인
	AI m_boss;//보스 ai
};