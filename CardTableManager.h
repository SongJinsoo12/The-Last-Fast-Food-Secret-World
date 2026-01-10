#pragma once
#include "Card.h"
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include <string>
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

	vector<GameCard*> GetRandomCard(int p_count);

	CAttribute StrToAit(string str);
	CType StrToType(string str);

protected:
	vector<Card*> m_AllCardDataVec;//전체 카드

};
