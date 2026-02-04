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
	type = Card::m_Atk;
}

int AtkCard::DefaultAtk(CAttribute attr, Star rank)
{
	if (attr == E_BREAD)
	{
		return 0;
	}
	return cardrule.RankValue(rank);
}

int AtkCard::Take_Damage_After_Atk(Mob& player, CAttribute attr, Star rank)
{
	if (player.GetMaxHP() > player.GetHP())
	{
		switch (rank)
		{
		case E_ONE:
			return 2;
		case E_TWO:
			return 3;
		case E_THREE:
			return 4;
		default:
			return 2;
		}
	}

	return 0;
}

int AtkCard::coinAtk(CAttribute attr, Star rank)
{
	int Cardcoin = rand() % 2;
	if (Cardcoin == 0)
	{
		return 0;
	}
	if (Cardcoin == 1)
	{
		switch (rank)
		{
		case E_ONE:
			return 2;
		case E_TWO:
			return 3;
		case E_THREE:
			return 4;
		default:
			return 2;
		}
	}
}

int AtkCard::DefCard_After_Atk(bool DefCardUsed, Star rank)
{
	if (DefCardUsed)
	{
		return m_Atk + 2;
	}
	return m_Atk;
}

int AtkCard::PoisonDamageCard(Star rank)
{
	if (Poison == false)
	{
		Poison = true;
		switch (rank)
		{
		case E_ONE:
			return 2;
		case E_TWO:
			return 3;
		case E_THREE:
			return 4;
		default:
			return 2;
		}
	}
	return DotDamage;
}


