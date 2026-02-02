#pragma once

#include "Card.h"   // CAttribute, CRank
class Mob;

// 방어카드: 수치(방어량/반사량 등)만 계산하는 역할.
// 실제 적용(방어막 저장, 피해 감소, 반사 적용 등)은 GameRunner/CardDatabase에서 처리하는 것을 권장합니다.
class DefCard : public Card
{
private:
    int type;
public:
    DefCard();

    // 기본 방어(속성이 BREAD면 0)
    int DefaultDef(CAttribute attr, CRank rank);

    // 빵 방어: "중립/범용" 방어량(현재는 랭크에 따른 소량의 고정 감소량 반환)
    int BreadDef(CAttribute atkAttr, CRank rank);

    // 무적(임시): 현재 Mob/Player에 '무적' 상태 시스템이 없다면 큰 방어값을 반환하는 방식으로 대체
    int invincibility(Mob& mob, CAttribute attr, CRank rank);

    // 피해 반사량(랭크별 2/3/4)
    int DamageReflection(CAttribute attr, CRank rank);
};
