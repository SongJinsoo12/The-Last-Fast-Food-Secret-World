#pragma once
#include "Card.h"
#include <vector>
#include <random>
using namespace std;


class CardTableManager
{

protected:
	static CardTableManager* m_Instance;
public:
	static CardTableManager* Instance();

public:
	CardTableManager();
	virtual ~CardTableManager();

	void Init();

	Card* GetCardData(int p_uid);
	Card* GetCardData(ALLCARDEnum p_uid);


	vector<Card*> GetRandomCard(int p_count);

protected:
	vector<Card*> m_AllCardDataVec;//전체 카드

};
