#pragma once
#include "DeckBuilding.h"
#include "MainGame.h"
#include "macroNum.h"
#include "Shop.h"
#include "CardTableManager.h"

class CardGacha
{
private:
	int gold_out;
	vector<Card> draw_card;
	bool isGachaFailed = false;
	bool isOneGacha;
	int index = 0;
	//전체카드풀
	GameCard* allCard[AllCARDMAXSIZE];
	BOOL isObtain[AllCARDMAXSIZE] = { FALSE, };
public:
	CardGacha()
	{
		//카드 id얻어오기
		for (int i = BASEATK + 1; i < ATKLIMIT; i++)
		{
			Card* card = CardTableManager::Instance()->GetCardData(i);
			allCard[index++] = new GameCard(card);
		}
		for (int i = BASEDEF + 1; i < DEFLIMIT; i++)
		{
			Card* card = CardTableManager::Instance()->GetCardData(i);
			allCard[index++] = new GameCard(card);
		}
		for (int i = BASEMAGIC + 1; i < MAGICLIMIT; i++)
		{
			Card* card = CardTableManager::Instance()->GetCardData(i);
			allCard[index++] = new GameCard(card);
		}
	}
	virtual ~CardGacha()
	{

	}

	void one(DeckBuilding& p_deck, MainGame& p_mg);//전체카드 중 1장을 인벤에저장
	void ten(DeckBuilding& p_deck, MainGame& p_mg);//전체카드 중 10장을 인벤에저장
	void GetGacha(bool isOne, DeckBuilding& p_deck, MainGame& p_mg, Chest p_selChest);//뽑기를 실행(T-1/F-10)
	void InGacha();//뽑기화면 속 로직

	//상점에 뽑기버튼을 출력, 버튼 클릭 시 뽑기를 실행
	void DrawGachaButton(HDC p_hdc, DeckBuilding p_deck, Chest p_selChest, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[]);
	//뽑기화면을 출력
	void DrawGacha(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[]);
};

extern CardGacha g_CardGacha;