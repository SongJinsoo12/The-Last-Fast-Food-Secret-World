<<<<<<< HEAD
ï»¿#pragma once
=======
#pragma once
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
#include <cstdlib>
#include <vector>
#include <string>
#include <time.h>
#include <Windows.h>
#include "Card.h"
#include "macroNum.h"
#include "RenderManager.h"

#include <iostream>
<<<<<<< HEAD
#include <fstream>	//íŒŒì¼ì½ê¸°
=======
#include <fstream>	//ÆÄÀÏÀĞ±â
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

#define BSIZE 25
double LenghtPts(int x1, int y1, int x2, int y2);
BOOL InCircle(int x, int y, int mx, int my);

class DeckBuilding
{
private:
<<<<<<< HEAD
	//í˜„ì¬ ë±
	vector<Card> myDeck;
	//ë³´ìœ í•œ ì¹´ë“œë“¤
=======
	//ÇöÀç µ¦
	vector<Card> myDeck;
	//º¸À¯ÇÑ Ä«µåµé
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
	vector<Card> inven;
	vector<Card> atkCards;
	vector<Card> defCards;
	vector<Card> magicCards;
<<<<<<< HEAD
	Card* SelectedCard;//ë±í¸ì§‘í™”ë©´ì—ì„œ ìœ ì €ê°€ ì •ë³´ë¥¼ í™•ì¸í•  ì¹´ë“œë³€ìˆ˜ / exitì‹œ ì„ íƒí•´ì œí• ê²ƒ

	int filter = 0;//í•„í„°ìš© ë³€ìˆ˜ 0ì´ë©´ í•„í„°x
	int Star_n[3] = { 0, };//ë±ì˜ 1,2,3ì„± ì¹´ë“œê°œìˆ˜
=======
	Card* SelectedCard;//µ¦ÆíÁıÈ­¸é¿¡¼­ À¯Àú°¡ Á¤º¸¸¦ È®ÀÎÇÒ Ä«µåº¯¼ö / exit½Ã ¼±ÅÃÇØÁ¦ÇÒ°Í

	int filter = 0;//ÇÊÅÍ¿ë º¯¼ö 0ÀÌ¸é ÇÊÅÍx
	int Star_n[3] = { 0, };//µ¦ÀÇ 1,2,3¼º Ä«µå°³¼ö
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
	int i_page = 0, max_page = 0;
	bool isShowHelper = false;

public:
	DeckBuilding()
	{

	}
	virtual ~DeckBuilding()
	{
	}

	void SaveDeck();
	void LoadDeck();

	void PageBuff(bool p_isIncrease)
	{
		if (p_isIncrease)
		{
			++i_page;
			if (i_page > max_page) --i_page;
		}
		else
		{
			--i_page;
			if (i_page < 0) i_page = 0;
		}
		cout << i_page << " / " << max_page << endl;
		return;
	}
<<<<<<< HEAD
	//ì¤‘ë³µì´ ì¡´ì¬í•˜ë©´ ì œê±°í›„ ë’¤ì˜ ì¹´ë“œë“¤ì„ ì•ìœ¼ë¡œ ì´ë™
	vector<Card> EraseDuple(vector<Card> p_cards);

	//ì¸ë²¤í† ë¦¬ì˜ ì‚¬ì´ì¦ˆ ë¦¬í„´
=======
	//Áßº¹ÀÌ Á¸ÀçÇÏ¸é Á¦°ÅÈÄ µÚÀÇ Ä«µåµéÀ» ¾ÕÀ¸·Î ÀÌµ¿
	vector<Card> EraseDuple(vector<Card> p_cards);

	//ÀÎº¥Åä¸®ÀÇ »çÀÌÁî ¸®ÅÏ
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
	int GetSize();
	void ChangeFilter()
	{
		++filter;
		if (filter > 3) filter = 0;
		cout << "filter : " << filter << endl;
	}

<<<<<<< HEAD
	//íŒŒë¼ë¯¸í„°ì˜ ë°°ì—´ì„ ì¸ë²¤ ë§¨ ë’¤ì— ì¶”ê°€í•¨. ì¤‘ë³µ ì œê±°ë„ í•´ì¤Œ
	void PushCard(vector<Card> p_cards);
	vector<Card> SetPos(vector<Card> p_cards)
	{
		//ì¸ë²¤ì— ë“¤ì–´ê°ˆ ìë¦¬ì— ë§ê²Œ ì¢Œí‘œë¥¼ ì„¸íŒ…í•¨.
=======
	//ÆÄ¶ó¹ÌÅÍÀÇ ¹è¿­À» ÀÎº¥ ¸Ç µÚ¿¡ Ãß°¡ÇÔ. Áßº¹ Á¦°Åµµ ÇØÁÜ
	void PushCard(vector<Card> p_cards);
	vector<Card> SetPos(vector<Card> p_cards)
	{
		//ÀÎº¥¿¡ µé¾î°¥ ÀÚ¸®¿¡ ¸Â°Ô ÁÂÇ¥¸¦ ¼¼ÆÃÇÔ.
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
		for (int i = 0; i < p_cards.size(); i++)
		{
			int x = ((i) % 25) % 5, y = ((i) % 25) / 5;
			p_cards[i].x = x * 82 + 1050, p_cards[i].y = y * 120 + 130;
			cout << p_cards[i].GetUid() << ": " << p_cards[i].x << " , " << p_cards[i].y << endl;
		}
		return p_cards;
	}
	vector<Card> SortCards(vector<Card> p_cards)
	{
		for (int i = 0; i < p_cards.size() - 1; i++)
		{
			for (int j = i + 1; j < p_cards.size(); j++)
			{
				if (p_cards[i].GetUid() > p_cards[j].GetUid())
				{
					swap(p_cards[i], p_cards[j]);
				}
			}
		}
		return p_cards;
	}

	void pushTypeCard(Card p_card)
	{
		CType type = p_card.GetType();
		switch (type)
		{
		case E_Attack:
			atkCards.push_back(p_card);
			break;
		case E_Deffense:
			defCards.push_back(p_card);
			break;
		case E_Magic:
			magicCards.push_back(p_card);
			break;
		}
	}

<<<<<<< HEAD
	//ì¸ë²¤->ë± (ë±, ë§ˆìš°ìŠ¤X, ë§ˆìš°ìŠ¤Y)
	void ItoD(int p_mx, int p_my);
	//ë±->ì¸ë²¤ (ë±, ë§ˆìš°ìŠ¤X, ë§ˆìš°ìŠ¤Y)
	void DtoI(int p_mx, int p_my);
	void SelectCard(int p_mx, int p_my);
	void DeckBuild(int p_mx, int p_my, char click_m);//click_m == ì¢Œ/ìš°í´ë¦­ í™•ì¸ìš©, ì¢Œ-ì¹´ë“œí•˜ì´ë¼ì´íŠ¸/ìš°-ì¹´ë“œì´ë™
=======
	//ÀÎº¥->µ¦ (µ¦, ¸¶¿ì½ºX, ¸¶¿ì½ºY)
	void ItoD(int p_mx, int p_my);
	//µ¦->ÀÎº¥ (µ¦, ¸¶¿ì½ºX, ¸¶¿ì½ºY)
	void DtoI(int p_mx, int p_my);
	void SelectCard(int p_mx, int p_my);
	void DeckBuild(int p_mx, int p_my, char click_m);//click_m == ÁÂ/¿ìÅ¬¸¯ È®ÀÎ¿ë, ÁÂ-Ä«µåÇÏÀÌ¶óÀÌÆ®/¿ì-Ä«µåÀÌµ¿
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f

	void EnterDeckBuild()
	{
		filter = 0;
		for (int i = 0; i < inven.size(); i++)
		{
			pushTypeCard(inven[i]);
		}

		atkCards = SetPos(atkCards);
		defCards = SetPos(defCards);
		magicCards = SetPos(magicCards);

		max_page = (inven.size() / 25);
		cout << "maxpage : " << max_page << endl;
	}
	void ExitDeckBuild()
	{
		filter = 0;
		for (int i = 0; i < inven.size(); i++) m_rend.RemoveIDIamage("inven_card" + to_string(i));
		for (int i = 0; i < myDeck.size(); i++) m_rend.RemoveIDIamage("deck_card" + to_string(i));
		m_rend.RemoveIDIamage("s_card");
	}

<<<<<<< HEAD
	//ì¸ë²¤í† ë¦¬ ì¶œë ¥
	void DrawInventory(HDC p_hdc, WCHAR p_text[], vector<Card> p_cardType);
	//ë§ˆì´ë± ì¶œë ¥
	void DrawMyDeck(HDC p_hdc, WCHAR p_text[]);
	//ë±ë¹Œë”© í™”ë©´ ì¶œë ¥
=======
	//ÀÎº¥Åä¸® Ãâ·Â
	void DrawInventory(HDC p_hdc, WCHAR p_text[], vector<Card> p_cardType);
	//¸¶ÀÌµ¦ Ãâ·Â
	void DrawMyDeck(HDC p_hdc, WCHAR p_text[]);
	//µ¦ºôµù È­¸é Ãâ·Â
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
	void DrawDeckBuild(HDC p_hdc, WCHAR p_text[]);
	void DrawHelp()
	{
		if (isShowHelper) m_rend.ImageVisible("helper", true);
		else m_rend.ImageVisible("helper", false);
	}
<<<<<<< HEAD
};

extern DeckBuilding g_DeckBuild;
=======
};
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
