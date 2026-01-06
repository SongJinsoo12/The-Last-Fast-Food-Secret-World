#include "CardTableManager.h"

CardTableManager* CardTableManager::m_Instance = nullptr;

CardTableManager* CardTableManager::Instnace()
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
}

void CardTableManager::Init()
{
	CardTableManager::Instnace();// manager;

	//manager.Init()

	m_AllCardDataVec.resize(3000);

	// 엑셀에서 읽어 와서 세팅하기
	// 

	// 테스트 소스
	Card* card = new Card((int)ALLCARDEnum::Attack01);
	card->SetAtk(10);
	m_AllCardDataVec[(int)ALLCARDEnum::Attack01] = card;

	m_AllCardDataVec[(int)ALLCARDEnum::Defense01] = new Card((int)ALLCARDEnum::Defense01);
}

Card* CardTableManager::GetCardData(int p_uid)
{
	return m_AllCardDataVec[p_uid];
}

Card* CardTableManager::GetCardData(ALLCARDEnum p_uid)
{
	return m_AllCardDataVec[(int)p_uid];
}

//덱 셔플
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
