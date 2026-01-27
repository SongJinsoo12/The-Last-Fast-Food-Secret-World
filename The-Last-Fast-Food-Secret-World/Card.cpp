#include "Card.h"


Card::Card()
{
	Init();
}

Card::Card(int p_uid)
{
	Init();
	uid = p_uid;
}

void Card::Init()
{
	uid = -1;
	atk = 0;
	def = 0;
	Ait = E_BREAD;
	Type = E_Attack;
	star = E_TWO;
	//SetImage();
}

int Card::GetUid()
{
	return uid;
}

void Card::SetUid(int p_uid)
{
	uid = p_uid;
}

int Card::GetAtk()
{
	return atk;
}

void Card::SetAtk(int p_atk)
{
	atk = p_atk;
}

int Card::GetDef()
{
	return def;
}

void Card::SetDef(int p_def)
{
	def = p_def;
}

CAttribute Card::GetAit()
{
	return Ait;
}

void Card::SetAit(CAttribute p_Ait)
{
	Ait = p_Ait;
}

CType Card::GetType()
{
	return Type;
}

void Card::SetType(CType p_Type)
{
	Type = p_Type;
}

Star Card::GetStar()
{
	return star;
}

void Card::SetStar(Star p_Star)
{
	star = p_Star;
}

GameCard::GameCard()
{
}

GameCard::GameCard(Card* p_Card)
{
	this->SetUid(p_Card->GetUid());
	this->SetAtk(p_Card->GetAtk());
	this->SetDef(p_Card->GetDef());
	this->SetAit(p_Card->GetAit());
	this->SetType(p_Card->GetType());
	this->SetStar(p_Card->GetStar());
}

GameCard::~GameCard()
{
}
