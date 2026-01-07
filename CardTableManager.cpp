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
	if (m_Instance)
	{
		delete m_Instance;
		m_Instance = nullptr;
	}
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
	//CardTableManager::Instance();// manager;

	//manager.Init()

	m_AllCardDataVec.resize(3000);

	// 엑셀에서 읽어 와서 세팅하기

	// 테스트 소스
	//공격 카드 로드
	for (size_t i = BASEATK + 1; i <= ATK02; i++)
	{
		Card* card = new Card(i);
		card->SetAtk(5 * i);
		card->SetDef(0);
		card->SetAit(CAttribute::E_BREAD);
		card->SetType(CType::E_Attack);
		m_AllCardDataVec[i] = card;
	}
	
	//방어 카드 로드
	for (size_t i = BASEDEF + 1; i <= DEF02; i++)
	{
		int index = i - BASEDEF;

		Card* card = new Card(i);
		card->SetAtk(0);
		card->SetDef(5 * index);
		card->SetAit(CAttribute::E_BREAD);
		card->SetType(CType::E_Deffense);
		m_AllCardDataVec[i] = card;
	}

	//특수 카드 로드
	for (size_t i = BASEMAGIC + 1; i <= MAGIC02; i++)
	{
		int index = i - BASEMAGIC;

		Card* card = new Card(i);
		card->SetAtk(0);
		card->SetDef(0);
		card->SetAit(CAttribute::E_BREAD);
		card->SetType(CType::E_Magic);
		m_AllCardDataVec[i] = card;
	}
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
