#include "AtkCard.h"
#include "CardRule.h"
#include "Mob.h"
#include <random>

CardRule cardrule;

AtkCard::AtkCard()
{
	TakeDamage = false;
	Damage = 2;
	DotDamage = 1;
	DefCardUsed = false;
	Poison = false;
	type = Card::Atk;
}

int AtkCard::DefaultAtk(CAttribute attr, CRank rank) 
{
	if (attr == E_BREAD)
	{
		return 0;
	}
	return cardrule.RankValue(rank);
}

int AtkCard::Take_Damage_After_Atk(Mob& player, CAttribute attr, CRank rank)
{
	if (player.GetMaxHP() > player.GetHP())
	{
		switch (rank)
		{
		case Star_1:
			return 2;
		case Star_2:
			return 3;
		case Star_3:
			return 4;
		default:
			return 2;
		}
	}

	return 0;
}

int AtkCard::coinAtk(CAttribute attr, CRank rank) 
{
	int coin = rand() % 2;
	if (coin == 0)
	{
		return 0;
	}
	if (coin == 1)
	{
		switch (rank)
		{
		case Star_1:
			return 2;
		case Star_2:
			return 3;
		case Star_3:
			return 4;
		default:
			return 2;
		}
	}
}

int AtkCard::DefCard_After_Atk(bool DefCardUsed, CRank rank)
{
	if (DefCardUsed)
	{
		return Atk + 2;
	}
	return Atk;
}

int AtkCard::PoisonDamageCard(CRank rank) 
{
	if (Poison == false)
	{
		Poison = true;
		switch (rank)
		{
		case Star_1:
			DotDamage = 1;
			break;
		case Star_2:
			DotDamage = 2;
			break;
		case Star_3:
			DotDamage = 3;
			break;
		default:
			break;
		}
	}
	return DotDamage;
}


