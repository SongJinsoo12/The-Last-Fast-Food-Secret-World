#pragma once
#include <vector>

#include "Card.h"
#define BOSSUID 250

using namespace std;

class Card;
class GameCard;

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
	Star StrToStar(string str);
	void CardImageLoad(int uid, CAttribute  ait, CType type, Star star);

protected:
	vector<Card*> m_AllCardDataVec;//ÀüÃ¼ Ä«µå

};