#include "Status.h"

CardPlayerStatus::CardPlayerStatus() : hp(50), isActAval(true)
{
}

CardPlayerStatus::~CardPlayerStatus()
{
}

int CardPlayerStatus::GetHP()
{
    return hp;
}

void CardPlayerStatus::SetHP(int p_hp)
{
	hp = p_hp;
}

bool CardPlayerStatus::GetIsActAval()
{
	return isActAval;
}

void CardPlayerStatus::SetIsActAval(bool p_isActAval)
{
	isActAval = p_isActAval;
}