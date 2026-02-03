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

	// csv 읽어 와서 로드하기
	ifstream file("All_Card.csv");
	if (!file.is_open())
		return;

	string line;
	getline(file, line); //첫 번째 줄 정보없음
	while (getline(file, line))
	{
		stringstream ss(line);
		string cell;
		vector<string> row;

		// 쉼표(,)를 기준으로 잘라서 row 벡터에 저장
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


		//카드 생성 및 데이터 세팅
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

//카드 셔플
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

		//중복 확인
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

	cout << "카드 셔플 확인\n";
	return outvec;
}

CAttribute CardTableManager::StrToAit(string str)
{
	if (str == "E_BULGOGI") return E_BULGOGI;
	if (str == "E_SOURCE") return E_SOURCE;
	if (str == "E_CHESSE") return E_CHESSE;
	if (str == "E_VEGAT") return E_VEGAT;
	if (str == "E_BREAD") return E_BREAD;
	return E_BREAD; // 기본값
}

CType CardTableManager::StrToType(string str)
{
	if (str == "E_Attack") return E_Attack;
	if (str == "E_Deffense") return E_Deffense;
	if (str == "E_Magic") return E_Magic;
	return E_Attack; // 기본값
}

Star CardTableManager::StrToStar(string str)
{
	if (str == "E_ONE") return E_ONE;
	if (str == "E_TWO") return E_TWO;
	if (str == "E_THREE") return E_THREE;
	return E_TWO; //기본값
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