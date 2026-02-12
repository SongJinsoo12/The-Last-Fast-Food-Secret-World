#pragma once
#include "Timer.h"
#include "Animation.h"
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
	void CardAct(CardManager& player, CardManager& opponent);

	void DrawLine(HDC hdc, int startX, int startY, int lengthX, int lengthY);
	void DrawBG();
	void DrawPlayerHand();
	void DrawOppHand();
	void HandSelect(CardManager& player, CardManager& opponent);
	void StartTurn(CardManager& player, CardManager& opponent);
	void TimeLimit(CardManager& player, CardManager& opponent);
	void BossCardAct(CardManager& player);
	void SetImage();
	void LoadMonoGram(string p_image, string p_id, int p_x, int p_y);
	void RemoveMonoGram(string p_id);
	void PuaseGame();
	
	bool* GetIsShiny();
	bool* GetIsRip();
	bool* GetIsSkill();
	bool* GetIsDef();


private:
	int m_DeckCount;//덱 장수
	int m_HandCount;//패 장수
	int m_HandSelection;//패 카드 선택
	vector<GameCard*> m_Hand;//패 카드
	vector<GameCard*> m_Deck;//덱 카드
	bool m_IsMyTurn;//턴 확인
	bool m_IsSelect;//패 카드 선택 확인
	//AI m_boss;//보스 ai
	Timer m_turnTime;
	bool m_isShiny, m_isRip, m_isSkill, m_isDef;
	int m_monoId;
};