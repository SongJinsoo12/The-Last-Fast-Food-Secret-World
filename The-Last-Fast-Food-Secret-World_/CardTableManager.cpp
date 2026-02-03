#include "CardTableManager.h"
#include "RenderManager.h"
#include "ImageLoad.h"
#include "ImageManager.h"
#include <random>
#include <fstream>
#include <sstream>
#include <string>

CardTableManager* CardTableManager::m_Instance = nullptr;

CardTableManager* CardTableManager::Instance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new CardTableManager();
	}

	return m_Instance;
}

CardTableManager::CardTableManager()
{
	Init();
}

CardTableManager::~CardTableManager()
{
	for (size_t i = 0; i < m_AllCardDataVec.size(); i++)
	{
		if (m_AllCardDataVec[i] != nullptr)
		{
			delete m_AllCardDataVec[i];
		}
	}
}

void CardTableManager::Init()
{
	m_AllCardDataVec.resize(250);

<<<<<<< HEAD
	// csv ÀÐ¾î ¿Í¼­ ·ÎµåÇÏ±â
=======
	// csv ì½ì–´ ì™€ì„œ ë¡œë“œí•˜ê¸°
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
	ifstream file("All_Card.csv");
	if (!file.is_open())
		return;

	string line;
<<<<<<< HEAD
	getline(file, line); //Ã¹ ¹øÂ° ÁÙ Á¤º¸¾øÀ½
=======
	getline(file, line); //ì²« ë²ˆì§¸ ì¤„ ì •ë³´ì—†ìŒ
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
	while (getline(file, line))
	{
		stringstream ss(line);
		string cell;
		vector<string> row;

<<<<<<< HEAD
		// ½°Ç¥(,)¸¦ ±âÁØÀ¸·Î Àß¶ó¼­ row º¤ÅÍ¿¡ ÀúÀå
=======
		// ì‰¼í‘œ(,)ë¥¼ ê¸°ì¤€ìœ¼ë¡œ ìž˜ë¼ì„œ row ë²¡í„°ì— ì €ìž¥
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
		while (getline(ss, cell, ',')) {
			row.push_back(cell);
		}

		if (row.size() < 6)
			continue;

		int uid = stoi(row[0]);
		int atk = stoi(row[1]);
		int def = stoi(row[2]);
		string aitStr = row[3];
		string typeStr = row[4];
		string starStr = row[5];


<<<<<<< HEAD
		//Ä«µå »ý¼º ¹× µ¥ÀÌÅÍ ¼¼ÆÃ
=======
		//ì¹´ë“œ ìƒì„± ë° ë°ì´í„° ì„¸íŒ…
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
		Card* newCard = new Card(uid);
		newCard->SetAtk(atk);
		newCard->SetDef(def);
		newCard->SetAit(StrToAit(aitStr));
		newCard->SetType(StrToType(typeStr));
		newCard->SetStar(StrToStar(starStr));
		m_AllCardDataVec[uid] = newCard;

		CardImageLoad(uid, StrToAit(aitStr), StrToType(typeStr), StrToStar(starStr));
		CardImageLoad(uid + BOSSUID, StrToAit(aitStr), StrToType(typeStr), StrToStar(starStr));
	}

	file.close();
}

Card* CardTableManager::GetCardData(int p_uid)
{
	return m_AllCardDataVec[p_uid];
}

Card* CardTableManager::GetCardData(ALLCARDEnum p_uid)
{
	return m_AllCardDataVec[(int)p_uid];
}

<<<<<<< HEAD
//Ä«µå ¼ÅÇÃ
=======
//ì¹´ë“œ ì…”í”Œ
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
vector<GameCard*> CardTableManager::GetRandomCard(int p_count)
{
	vector<GameCard*> outvec;
	vector<int> noSameUid;

	for (size_t i = 0; i < p_count; i++)
	{
		int randUid;
		randomInit(1, 232);
		randUid = cookRandom(gen);
		Card* card = GetCardData(randUid);

<<<<<<< HEAD
		//Áßº¹ È®ÀÎ
=======
		//ì¤‘ë³µ í™•ì¸
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
		for (size_t j = 0; j < noSameUid.size(); j++)
		{
			if (randUid == noSameUid[j])
			{
				--i;
				card = nullptr;
				break;
			}
		}
		if (card != nullptr)
		{
			Card* card = CardTableManager::Instance()->GetCardData(randUid);
			GameCard* gameCard = new GameCard(card);

			outvec.push_back(gameCard);
			noSameUid.push_back(randUid);
		}
		else
		{
			--i;
		}
	}

<<<<<<< HEAD
	cout << "Ä«µå ¼ÅÇÃ È®ÀÎ\n";
=======
	cout << "ì¹´ë“œ ì…”í”Œ í™•ì¸\n";
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
	return outvec;
}

CAttribute CardTableManager::StrToAit(string str)
{
	if (str == "E_BULGOGI") return E_BULGOGI;
	if (str == "E_SOURCE") return E_SOURCE;
	if (str == "E_CHESSE") return E_CHESSE;
	if (str == "E_VEGAT") return E_VEGAT;
	if (str == "E_BREAD") return E_BREAD;
<<<<<<< HEAD
	return E_BREAD; // ±âº»°ª
=======
	return E_BREAD; // ê¸°ë³¸ê°’
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
}

CType CardTableManager::StrToType(string str)
{
	if (str == "E_Attack") return E_Attack;
	if (str == "E_Deffense") return E_Deffense;
	if (str == "E_Magic") return E_Magic;
<<<<<<< HEAD
	return E_Attack; // ±âº»°ª
=======
	return E_Attack; // ê¸°ë³¸ê°’
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
}

Star CardTableManager::StrToStar(string str)
{
	if (str == "E_ONE") return E_ONE;
	if (str == "E_TWO") return E_TWO;
	if (str == "E_THREE") return E_THREE;
<<<<<<< HEAD
	return E_TWO; //±âº»°ª
=======
	return E_TWO; //ê¸°ë³¸ê°’
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
}

void CardTableManager::CardImageLoad(int uid, CAttribute  ait, CType type, Star star)
{
	switch (type)
	{
	case E_Attack:
		switch (star)
		{
		case E_ONE:
			m_rend.SetImage(L"card_atk_1.png", to_string(uid),
				Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0), false,
				GameImage_M::LayerType::Card);
			break;
		case E_TWO:
			m_rend.SetImage(L"card_atk_2.png", to_string(uid),
				Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0), false,
				GameImage_M::LayerType::Card);
			break;
		case E_THREE:
			m_rend.SetImage(L"card_atk_3.png", to_string(uid),
				Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0), false,
				GameImage_M::LayerType::Card);
			break;
		}
		break;
	case E_Deffense:
		switch (star)
		{
		case E_ONE:
			m_rend.SetImage(L"card_def_1.png", to_string(uid),
				Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0), false,
				GameImage_M::LayerType::Card);
			break;
		case E_TWO:
			m_rend.SetImage(L"card_def_2.png", to_string(uid),
				Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0), false,
				GameImage_M::LayerType::Card);
			break;
		case E_THREE:
			m_rend.SetImage(L"card_def_3.png", to_string(uid),
				Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0), false,
				GameImage_M::LayerType::Card);
			break;
		}
		break;
	case E_Magic:
		m_rend.SetImage(L"card_magic.png", to_string(uid),
			Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0), false,
			GameImage_M::LayerType::Card);
		break;
	}

	switch (ait)
	{
	case E_BULGOGI:
		break;
	case E_SOURCE:
		break;
	case E_CHESSE:
		break;
	case E_VEGAT:
		break;
	case E_BREAD:
		break;
	}
}