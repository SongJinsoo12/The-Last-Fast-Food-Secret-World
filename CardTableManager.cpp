#include "CardTableManager.h"

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

		if (row.size() < 5) 
			continue;

		int uid = stoi(row[0]);
		int atk = stoi(row[1]);
		int def = stoi(row[2]);
		string aitStr = row[3];
		string typeStr = row[4];


		//카드 생성 및 데이터 세팅
		Card* newCard = new Card(uid);
		newCard->SetAtk(atk);
		newCard->SetDef(def);
		newCard->SetAit(StrToAit(aitStr));
		newCard->SetType(StrToType(typeStr));
		m_AllCardDataVec[uid] = newCard;
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
vector<Card*> CardTableManager::GetRandomCard(int p_count)
{
	vector<Card*> outvec;

	for (size_t i = 0; i < p_count; i++)
	{
		int randUid;
		randomInit(0, 10);
		randUid = cookRandom(gen);
		Card* card = GetCardData(randUid);
		if (card != nullptr)
		{
			outvec.push_back(card);
		}
		else
		{
			--i;
		}
	}

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
	if (str == "E_Deffence") return E_Deffense;
	if (str == "E_Magic") return E_Magic;
	return E_Attack; // 기본값
}
