#include "CardRule.h"

CardRule::CardRule()
{
	Status = false;
}

int CardRule::RankValue(Star rank)
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

int CardRule::RankMatchUp(Star Atk, Star Def)
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
