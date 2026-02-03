#include "Status.h"

Status::Status() : hp(50), isActAval(true)
{
}

Status::~Status()
{
}

int Status::GetHP()
{
    return hp;
}

void Status::SetHP(int p_hp)
{
	hp = p_hp;
}

bool Status::GetIsActAval()
{
	return isActAval;
}

void Status::SetIsActAval(bool p_isActAval)
{
	isActAval = p_isActAval;
}