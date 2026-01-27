#include "DefCard.h"
#include "CardRule.h"
#include "Mob.h"

// AtkCard.cpp와 동일하게 "카드 효과 수치 계산"에서 CardRule을 사용
static CardRule g_rule;

DefCard::DefCard() 
{
    type = Card::Rdc;
}

// ----------------------
// 기본 방어(☆1/☆2/☆3)
// ----------------------
int DefCard::DefaultDef(CAttribute attr, CRank rank)
{
    // 빵 속성은 '무효/중립' 취급(원하면 규칙 변경 가능)
    if (attr == E_BREAD)
        return 0;

    // 프로젝트에서 RankValue(☆1=5, ☆2=10, ☆3=15)를 이미 쓰고 있으니
    // 기본 방어도 동일하게 반환(방어막/감소 시스템이 생기면 그대로 활용 가능)
    return g_rule.RankValue(rank);
}

// ----------------------
// 빵 방어(중립 감소)
// ----------------------
// 원본 틀에 있던 함수로 보이며, 현재 구조에서는 '들어오는 피해량' 정보가 없으므로
// 랭크에 따른 "고정 감소량"을 반환하도록 구현했습니다.
// (실제 적용은: 받은 피해 - BreadDef(...) 같은 형태로 사용하세요)
int DefCard::BreadDef(CAttribute /*atkAttr*/, CRank rank)
{
    switch (rank)
    {
    case Star_1: return 1;
    case Star_2: return 2;
    case Star_3: return 3;
    default:     return 1;
    }
}

// ----------------------
// 무적(임시 구현)
// ----------------------
// 현재 Mob/Player 클래스에 "무적 턴" 같은 상태 필드가 없다면
//  - (추천) Mob/Player에 invincibleTurns 같은 상태를 추가하고,
//          여기서는 그 값을 세팅하는 방식이 가장 깔끔합니다.
//  - 지금은 "엄청 큰 방어값"을 반환해서, 적용부에서 그 턴 피해가 0이 되도록 처리할 수 있게 했습니다.
int DefCard::invincibility(Mob& /*mob*/, CAttribute attr, CRank rank)
{
    if (attr == E_BREAD)
        return 0;

    switch (rank)
    {
    case Star_1: return 9999;
    case Star_2: return 9999;
    case Star_3: return 9999;
    default:     return 9999;
    }
}

// ----------------------
// 피해 반사(☆1/☆2/☆3)
// ----------------------
int DefCard::DamageReflection(CAttribute /*attr*/, CRank rank)
{
    switch (rank)
    {
    case Star_1: return 2;
    case Star_2: return 3;
    case Star_3: return 4;
    default:     return 2;
    }
}
