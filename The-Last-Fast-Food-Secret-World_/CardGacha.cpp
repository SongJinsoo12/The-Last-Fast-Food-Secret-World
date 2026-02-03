<<<<<<< HEAD
ï»¿#include "CardGacha.h"

void CardGacha::one(DeckBuilding& p_deck, MainGame& p_mg)
{
	int invensize = p_deck.GetSize();//ì¹´ë“œí’€ì—ì„œ ëžœë¤í•œ ì¹´ë“œë¥¼ ë½‘ìŒ
=======
#include "CardGacha.h"

void CardGacha::one(DeckBuilding& p_deck, MainGame& p_mg)
{
	int invensize = p_deck.GetSize();//Ä«µåÇ®¿¡¼­ ·£´ýÇÑ Ä«µå¸¦ »ÌÀ½
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
	draw_card.resize(1);
	int randIdx = rand() % AllCARDMAXSIZE;
	int ID = allCard[randIdx]->GetUid();
	draw_card[0].SetUid(ID);
	Star STAR = allCard[randIdx]->GetStar();
	draw_card[0].SetStar(STAR);
	CType TYPE = allCard[randIdx]->GetType();
	draw_card[0].SetType(TYPE);

<<<<<<< HEAD
	//ì¹´ë“œê°€ ì¶”ê°€ë ë•Œ ì¤‘ë³µì¹´ë“œëŠ” ë½‘ê¸°ê²°ê³¼ì—ì„œ ë‚˜ì˜¤ë˜, ì¸ë²¤ì—ì„œ ì œì™¸ë˜ì–´ì•¼í•¨.
	if (isObtain[randIdx])
	{
		//ì¤‘ë³µ ì‹œ ìž„ì‹œë¡œ ì•„ì´ë””ë¥¼ ìŒìˆ˜ë¡œ ë³€ê²½ (ê¸°ë³¸ì¹´ë“œì˜ ê²½ìš°ëŠ” ì˜ˆì™¸ë¥¼ ì„¤ì •í•  ì˜ˆì •)
		cout << draw_card[0].GetUid() << "ëŠ” ì¤‘ë³µ" << "\n";
		draw_card[0].SetUid(-1);
		p_mg.AddGold(100 * 4); //ê³ ì •ê°’ * ë ˆì–´ë„ë³„ ê°€ì¤‘ì¹˜ íŽ˜ì´ë°±
	}
	else
	{
		//ì‹ ê·œ íšë“ ì¹´ë“œëŠ” ì–»ì—ˆë‹¤ëŠ” í‘œì‹œë¥¼ ì„¤ì •
		cout << draw_card[0].GetUid() << "ëŠ” ì‹ ê·œ" << "\n";
=======
	//Ä«µå°¡ Ãß°¡µÉ¶§ Áßº¹Ä«µå´Â »Ì±â°á°ú¿¡¼­ ³ª¿ÀµÇ, ÀÎº¥¿¡¼­ Á¦¿ÜµÇ¾î¾ßÇÔ.
	if (isObtain[randIdx])
	{
		//Áßº¹ ½Ã ÀÓ½Ã·Î ¾ÆÀÌµð¸¦ À½¼ö·Î º¯°æ (±âº»Ä«µåÀÇ °æ¿ì´Â ¿¹¿Ü¸¦ ¼³Á¤ÇÒ ¿¹Á¤)
		cout << draw_card[0].GetUid() << "´Â Áßº¹" << "\n";
		draw_card[0].SetUid(-1);
		p_mg.AddGold(100 * 4); //°íÁ¤°ª * ·¹¾îµµº° °¡ÁßÄ¡ ÆäÀÌ¹é
	}
	else
	{
		//½Å±Ô È¹µæ Ä«µå´Â ¾ò¾ú´Ù´Â Ç¥½Ã¸¦ ¼³Á¤
		cout << draw_card[0].GetUid() << "´Â ½Å±Ô" << "\n";
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
		isObtain[randIdx] = TRUE;
	}
	int x = invensize % 5, y = invensize / 5;
	draw_card[0].x = x * 75 + 1050, draw_card[0].y = y * 150 + 50;
	p_deck.PushCard(draw_card);
}

void CardGacha::ten(DeckBuilding& p_deck, MainGame& p_mg)
{
	int invensize = p_deck.GetSize();
	draw_card.resize(10);
	for (int i = 0; i < 10; i++)
	{
<<<<<<< HEAD
		//ì¹´ë“œí’€ì—ì„œ ëžœë¤í•œ ì¹´ë“œë¥¼ ë½‘ìŒ
=======
		//Ä«µåÇ®¿¡¼­ ·£´ýÇÑ Ä«µå¸¦ »ÌÀ½
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
		int randIdx = rand() % AllCARDMAXSIZE;
		int ID = allCard[randIdx]->GetUid();
		draw_card[i].SetUid(ID);
		Star STAR = allCard[randIdx]->GetStar();
		draw_card[i].SetStar(STAR);
		CType TYPE = allCard[randIdx]->GetType();
		draw_card[i].SetType(TYPE);

<<<<<<< HEAD
		//ì¹´ë“œê°€ ì¶”ê°€ë ë•Œ ì¤‘ë³µì¹´ë“œëŠ” ë½‘ê¸°ê²°ê³¼ì—ì„œ ë‚˜ì˜¤ë˜, ì¸ë²¤ì—ì„œ ì œì™¸ë˜ì–´ì•¼í•¨.
		if (isObtain[randIdx])
		{
			//ì¤‘ë³µ ì‹œ ìž„ì‹œë¡œ ì•„ì´ë””ë¥¼ ìŒìˆ˜ë¡œ ë³€ê²½ (ê¸°ë³¸ì¹´ë“œì˜ ê²½ìš°ëŠ” ì˜ˆì™¸ë¥¼ ì„¤ì •í•  ì˜ˆì •)
			cout << draw_card[i].GetUid() << "ëŠ” ì¤‘ë³µ" << "\n";
			draw_card[i].SetUid(-1);
			p_mg.AddGold(100 * 4); //ê³ ì •ê°’ * ë ˆì–´ë„ë³„ ê°€ì¤‘ì¹˜ íŽ˜ì´ë°±
		}
		else
		{
			//ì‹ ê·œ íšë“ ì¹´ë“œëŠ” ì–»ì—ˆë‹¤ëŠ” í‘œì‹œë¥¼ ì„¤ì •
			cout << draw_card[i].GetUid() << "ëŠ” ì‹ ê·œ" << "\n";
=======
		//Ä«µå°¡ Ãß°¡µÉ¶§ Áßº¹Ä«µå´Â »Ì±â°á°ú¿¡¼­ ³ª¿ÀµÇ, ÀÎº¥¿¡¼­ Á¦¿ÜµÇ¾î¾ßÇÔ.
		if (isObtain[randIdx])
		{
			//Áßº¹ ½Ã ÀÓ½Ã·Î ¾ÆÀÌµð¸¦ À½¼ö·Î º¯°æ (±âº»Ä«µåÀÇ °æ¿ì´Â ¿¹¿Ü¸¦ ¼³Á¤ÇÒ ¿¹Á¤)
			cout << draw_card[i].GetUid() << "´Â Áßº¹" << "\n";
			draw_card[i].SetUid(-1);
			p_mg.AddGold(100 * 4); //°íÁ¤°ª * ·¹¾îµµº° °¡ÁßÄ¡ ÆäÀÌ¹é
		}
		else
		{
			//½Å±Ô È¹µæ Ä«µå´Â ¾ò¾ú´Ù´Â Ç¥½Ã¸¦ ¼³Á¤
			cout << draw_card[i].GetUid() << "´Â ½Å±Ô" << "\n";
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
			isObtain[randIdx] = TRUE;
		}
	}
	p_deck.PushCard(draw_card);
}

<<<<<<< HEAD
//ë½‘ê¸°ë¥¼ í•¨(TRUE-1ë½‘ / FALSE-10ë½‘, ë½‘ì€ì¹´ë“œë¥¼ ì €ìž¥í•  ë³€ìˆ˜)
=======
//»Ì±â¸¦ ÇÔ(TRUE-1»Ì / FALSE-10»Ì, »ÌÀºÄ«µå¸¦ ÀúÀåÇÒ º¯¼ö)
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
void CardGacha::GetGacha(bool isOne, DeckBuilding& p_deck, MainGame& p_mg, Chest p_selChest)
{
	isOneGacha = isOne;
	int remove_gold = p_selChest.GetPrice();
<<<<<<< HEAD
	//ë¶„ê¸°ë‚˜ëˆ ì„œ 1ì°¨ or 10ì°¨ ë‚˜ëˆ„ê¸°
=======
	//ºÐ±â³ª´²¼­ 1Â÷ or 10Â÷ ³ª´©±â
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
	if (isOne)
	{
		if (!p_mg.RemoveGold(remove_gold))
		{
			this->isGachaFailed = true;
		}
		one(p_deck, p_mg);
	}
	else if (!isOne)
	{
		if (!p_mg.RemoveGold(remove_gold * 9))
		{
			this->isGachaFailed = true;
		}
		ten(p_deck, p_mg);
	}
}

void CardGacha::InGacha()
{
<<<<<<< HEAD
	//ë½‘ê¸°ì˜ ì¢…ë¥˜ì— ë”°ë¼ ì¶œë ¥ë°©ì‹ ë³€ê²½
=======
	//»Ì±âÀÇ Á¾·ù¿¡ µû¶ó Ãâ·Â¹æ½Ä º¯°æ
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
	if (isOneGacha)
	{
		draw_card[0].x = 700, draw_card[0].y = 350;
	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			draw_card[i].x = (i % 5) * 250 + 200, draw_card[i].y = (i / 5) * 250 + 200;
		}
	}
}

<<<<<<< HEAD
void CardGacha::DrawGachaButton(HDC p_hdc, DeckBuilding p_deck, Chest p_selChest, int p_mx, int p_my, WCHAR p_text[])
{
	//1ë½‘ ë²„íŠ¼
	Rectangle(p_hdc, 700, 615, 1000, 685);
	wsprintf(p_text, TEXT("1ê°œ - %dG"), p_selChest.GetPrice());
	TextOut(p_hdc, 830, 645, p_text, lstrlen(p_text));

	//10ë½‘ ë²„íŠ¼
	Rectangle(p_hdc, 1050, 615, 1350, 685);
	wsprintf(p_text, TEXT("10ê°œ - %dG"), p_selChest.GetPrice() * 9);
=======
void CardGacha::DrawGachaButton(HDC p_hdc, DeckBuilding p_deck, Chest p_selChest, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[])
{
	//1»Ì ¹öÆ°
	if (InCircle(850, 635, p_mx, p_my))
	{
		p_hpen = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
		p_oldpen = (HPEN)SelectObject(p_hdc, p_hpen);
	}
	Rectangle(p_hdc, 700, 615, 1000, 685);
	SelectObject(p_hdc, p_oldpen);
	DeleteObject(p_hpen);	//Ææ»ý¼º / Ææ¿ø»óº¹±¸
	wsprintf(p_text, TEXT("1°³ - %dG"), p_selChest.GetPrice());
	TextOut(p_hdc, 830, 645, p_text, lstrlen(p_text));

	//10»Ì ¹öÆ°
	if (InCircle(1200, 635, p_mx, p_my))
	{
		p_hpen = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
		p_oldpen = (HPEN)SelectObject(p_hdc, p_hpen);
	}
	Rectangle(p_hdc, 1050, 615, 1350, 685);
	SelectObject(p_hdc, p_oldpen);
	DeleteObject(p_hpen);	//Ææ»ý¼º / Ææ¿ø»óº¹±¸
	wsprintf(p_text, TEXT("10°³ - %dG"), p_selChest.GetPrice() * 9);
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
	TextOut(p_hdc, 1170, 645, p_text, lstrlen(p_text));

	if (this->isGachaFailed)
	{
<<<<<<< HEAD
		TextOut(p_hdc, 50, 500, TEXT("ëˆì´ ë¶€ì¡±í•©ë‹ˆë‹¤."), 10);
	}
}

void CardGacha::DrawGacha(HDC p_hdc, int p_mx, int p_my, WCHAR p_text[])
{
	//ë½‘ì€ì¹´ë“œ ì¶œë ¥í•˜ê²Œ ìˆ˜ì •í• ê²ƒ///////////////////
=======
		TextOut(p_hdc, 50, 500, TEXT("µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù."), 10);
	}
}

void CardGacha::DrawGacha(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[])
{
	//»ÌÀºÄ«µå Ãâ·ÂÇÏ°Ô ¼öÁ¤ÇÒ°Í///////////////////
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
	for (int i = 0; i < draw_card.size(); i++)
	{
		Rectangle(p_hdc, draw_card[i].x - 45, draw_card[i].y - 75, draw_card[i].x + 45, draw_card[i].y + 75);
		wsprintf(p_text, TEXT("%d"), draw_card[i].GetUid());
		TextOut(p_hdc, draw_card[i].x - 2, draw_card[i].y, p_text, lstrlen(p_text));
	}

<<<<<<< HEAD
	wsprintf(p_text, TEXT("ì™¼ìª½ìƒë‹¨ì˜ ìƒì  ë²„íŠ¼ì„ ëˆŒëŸ¬ ëŒì•„ê°€ê¸°"));
	TextOut(p_hdc, 690, 650, p_text, lstrlen(p_text));
}

CardGacha g_Gacha;
=======
	wsprintf(p_text, TEXT("¿ÞÂÊ»ó´ÜÀÇ »óÁ¡ ¹öÆ°À» ´­·¯ µ¹¾Æ°¡±â"));
	TextOut(p_hdc, 690, 650, p_text, lstrlen(p_text));
}
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
