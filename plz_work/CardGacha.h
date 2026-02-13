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
	vector<GameCard> allCard;
	//GameCard* allCard = new GameCard[AllCARDMAXSIZE];
	BOOL isObtain[AllCARDMAXSIZE] = { FALSE, };
	int cookie_num = 0;

public:
	ButtonManager m_GachaBtnM;

public:
	CardGacha()
	{
		allCard.resize(AllCARDMAXSIZE);
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

		std::ifstream file("Deck.json", std::ios::in);//같은 경로내에서 파일불러오기

		//파일 불러오기확인용
		if (!file.is_open())
		{
			std::cerr << "Error opening Deck.json.\n";
			return;
		}

		//읽어들인 파일을 json에 저장
		json j;
		try
		{
			file >> j;
		}
		catch (const std::exception& e)
		{
			cerr << "json parcing error : " << e.what() << "\n";
			return;
		}

		if (j.contains("Cards_D") && j["Cards_D"].is_array())
		{
			//"Cards_D" 배열이 끝날때까지 반복
			for (const auto& card : j["Cards_D"])
			{
				int uid = -1;
				//uid키에 값이 존재하면 int로 받아옴
				if (card.contains("uid"))
				{
					try { uid = card["uid"].get<int>(); }
					catch (...) { uid = -1; }
				}

				Card c;
				if (uid >= 0) c = Card(uid);
				else c = Card();

				int c_uid = c.GetUid(), index = -1;
				switch ((int)(c_uid / 100))
				{
				case 0: isObtain[c.GetUid() - 1] = true; break;
				case 1: isObtain[c.GetUid() - 59] = true; break;
				case 2: isObtain[c.GetUid() - 116] = true; break;
				default: break;
				}
			}
		}

		if (j.contains("Cards_I") && j["Cards_I"].is_array())
		{
			//"Cards_D" 배열이 끝날때까지 반복
			for (const auto& card : j["Cards_I"])
			{
				int uid = -1;
				//uid키에 값이 존재하면 int로 받아옴
				if (card.contains("uid"))
				{
					try { uid = card["uid"].get<int>(); }
					catch (...) { uid = -1; }
				}

				Card c;
				if (uid >= 0) c = Card(uid);
				else c = Card();

				int c_uid = c.GetUid();
				switch ((int)(c_uid / 100))
				{
				case 0: isObtain[c.GetUid() - 1] = true; break;
				case 1: isObtain[c.GetUid() - 59] = true; break;
				case 2: isObtain[c.GetUid() - 116] = true; break;
				default: break;
				}
			}
		}
	}
	virtual ~CardGacha()
	{

	}

	vector<GameCard> GetAllCard()
	{
		return allCard;
	}
	void one(DeckBuilding& p_deck, MainGame& p_mg);//전체카드 중 1장을 인벤에저장
	void ten(DeckBuilding& p_deck, MainGame& p_mg);//전체카드 중 10장을 인벤에저장
	void GetGacha(bool isOne, DeckBuilding& p_deck, MainGame& p_mg, Chest p_selChest);//뽑기를 실행(T-1/F-10)
	void InGacha();//뽑기화면 속 로직

	void EnterGacha();
	//뽑기화면을 출력
	void DrawGacha(HDC p_hdc, int p_mx, int p_my, WCHAR p_text[]);
	void ExitGacha();
};

extern CardGacha g_Gacha; // 전역 카드뽑기 객체