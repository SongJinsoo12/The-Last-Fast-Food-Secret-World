<<<<<<< HEAD
ï»¿#pragma once
=======
#pragma once
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
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
<<<<<<< HEAD
	//ì „ì²´ì¹´ë“œí’€
=======
	//ÀüÃ¼Ä«µåÇ®
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
	GameCard* allCard[AllCARDMAXSIZE];
	BOOL isObtain[AllCARDMAXSIZE] = { FALSE, };
public:
	CardGacha()
	{
<<<<<<< HEAD
		//ì¹´ë“œ idì–»ì–´ì˜¤ê¸°
=======
		//Ä«µå id¾ò¾î¿À±â
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
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

<<<<<<< HEAD
	void one(DeckBuilding& p_deck, MainGame& p_mg);//ì „ì²´ì¹´ë“œ ì¤‘ 1ìž¥ì„ ì¸ë²¤ì—ì €ìž¥
	void ten(DeckBuilding& p_deck, MainGame& p_mg);//ì „ì²´ì¹´ë“œ ì¤‘ 10ìž¥ì„ ì¸ë²¤ì—ì €ìž¥
	void GetGacha(bool isOne, DeckBuilding& p_deck, MainGame& p_mg, Chest p_selChest);//ë½‘ê¸°ë¥¼ ì‹¤í–‰(T-1/F-10)
	void InGacha();//ë½‘ê¸°í™”ë©´ ì† ë¡œì§

	//ìƒì ì— ë½‘ê¸°ë²„íŠ¼ì„ ì¶œë ¥, ë²„íŠ¼ í´ë¦­ ì‹œ ë½‘ê¸°ë¥¼ ì‹¤í–‰
	void DrawGachaButton(HDC p_hdc, DeckBuilding p_deck, Chest p_selChest, int p_mx, int p_my, WCHAR p_text[]);
	//ë½‘ê¸°í™”ë©´ì„ ì¶œë ¥
	void DrawGacha(HDC p_hdc, int p_mx, int p_my, WCHAR p_text[]);
};

extern CardGacha g_Gacha;
=======
	void one(DeckBuilding& p_deck, MainGame& p_mg);//ÀüÃ¼Ä«µå Áß 1ÀåÀ» ÀÎº¥¿¡ÀúÀå
	void ten(DeckBuilding& p_deck, MainGame& p_mg);//ÀüÃ¼Ä«µå Áß 10ÀåÀ» ÀÎº¥¿¡ÀúÀå
	void GetGacha(bool isOne, DeckBuilding& p_deck, MainGame& p_mg, Chest p_selChest);//»Ì±â¸¦ ½ÇÇà(T-1/F-10)
	void InGacha();//»Ì±âÈ­¸é ¼Ó ·ÎÁ÷

	//»óÁ¡¿¡ »Ì±â¹öÆ°À» Ãâ·Â, ¹öÆ° Å¬¸¯ ½Ã »Ì±â¸¦ ½ÇÇà
	void DrawGachaButton(HDC p_hdc, DeckBuilding p_deck, Chest p_selChest, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[]);
	//»Ì±âÈ­¸éÀ» Ãâ·Â
	void DrawGacha(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[]);
};
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
