#include "Card.h"


Card::Card()
{
	Init();
}

Card::Card(int p_uid)
{
	Init();
	m_Uid = p_uid;
}

void Card::Init()
{
	m_Uid = -1;
	m_Atk = 0;
	m_Def = 0;
	m_Ait = E_BREAD;
	m_Type = E_Attack;
	m_Star = E_TWO;
	//SetImage();
}

int Card::GetUid()
{
	return m_Uid;
}

void Card::SetUid(int p_uid)
{
	m_Uid = p_uid;
}

int Card::GetAtk()
{
	return m_Atk;
}

void Card::SetAtk(int p_atk)
{
	m_Atk = p_atk;
}

int Card::GetDef()
{
	return m_Def;
}

void Card::SetDef(int p_def)
{
	m_Def = p_def;
}

CAttribute Card::GetAit()
{
	return m_Ait;
}

void Card::SetAit(CAttribute p_Ait)
{
	m_Ait = p_Ait;
}

CType Card::GetType()
{
	return m_Type;
}

void Card::SetType(CType p_Type)
{
	m_Type = p_Type;
}

Star Card::GetStar()
{
	return m_Star;
}

void Card::SetStar(Star p_Star)
{
	m_Star = p_Star;
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
