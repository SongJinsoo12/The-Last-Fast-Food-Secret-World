#pragma once
#include "Card.h"
#include "CardRule.h"
#include "Mob.h"

//공격 5 / 10 / 15
//? 피해 / 5 / 10 / 15
//ㅊ. ? 피해 후 공격카드를 사용한 직후 사용 가능(사용회수x).공격피해가 0. ? 배로 변경(소수점은 버림림). 0.25 / 0.5 / 0.75
//코인 토스로 데미지 적용 ex) 성공 시 데미지, 실패 시 불발
//상대가 방어 카드를 사용했을 시에만 데미지를 주는 카드
//스플릿 데미지(상태이상 = ? 턴 동안 출혈 ? 독 ? ) 1 / 2 / 3


class AtkCard : public Card
{
public:
	int Damage;
	int DotDamage;
	bool TakeDamage; // 데미지를 입었는지 안입었는지 확인
	bool DefCardUsed;
	int type;

public:
	bool Poison; // 독 이 있는지 없는지 확인

	AtkCard();
	virtual ~AtkCard() = default;

	// 기본 공격 카드
	int DefaultAtk(CAttribute attr, CRank rank);
	// 피해 후 공격카드를 사용한 직후 사용 가능 카드
	int Take_Damage_After_Atk(Mob& player, CAttribute attr, CRank rank);
	// 코인 토스로 데미지 적용
	int coinAtk(CAttribute attr, CRank rank);
	// 상개가 방어 카드 사용시 공격 가능
	int DefCard_After_Atk(bool DefCardUsed, CRank rank);
	// 도트데미지 (3턴동안 데미지를 줌)
	int PoisonDamageCard(CRank rank);
};
