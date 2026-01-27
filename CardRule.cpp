#include "CardRule.h"

CardRule::CardRule()
{
	Status = false;
}

int CardRule::RankValue(CRank rank)
{
	switch (rank)
	{
	case Star_1:
		return 5;
	case Star_2:
		return 10;
	case Star_3:
		return 15;
	default:
		return 1;
	}
}

int CardRule::RankMatchUp(CRank Atk, CRank Def)
{
	if (Atk <= Def)
	{
		return 0;
	}
	if (Atk > Def)
	{
		return Atk - Def;
	}
}

bool CardRule::IsCounterAttribute(CAttribute Atk, CAttribute Def)
{
	if (Def == E_BREAD || Atk == E_BREAD)
	{
		return false;
	}

	switch (Def)
	{
	case E_BULGOGI:
		return Atk == E_VEGAT;
	case E_SOURCE:
		return Atk == E_BULGOGI;
	case E_CHESSE:
		return Atk == E_SOURCE;
	case E_VEGAT:
		return Atk == E_CHESSE;
	default:
		return false;
	}
}
