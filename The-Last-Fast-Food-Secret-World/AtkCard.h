#pragma once
#include "Card.h"
#include "BattleTypes.h"
#include <random>

//공격 5 / 10 / 15
//? 피해 / 5 / 10 / 15
//ㅊ. ? 피해 후 공격카드를 사용한 직후 사용 가능(사용회수x).공격피해가 0. ? 배로 변경(소수점은 버림림). 0.25 / 0.5 / 0.75
//코인 토스로 데미지 적용 ex) 성공 시 데미지, 실패 시 불발
//상대가 방어 카드를 사용했을 시에만 데미지를 주는 카드
//스플릿 데미지(상태이상 = ? 턴 동안 출혈 ? 독 ? ) 1 / 2 / 3

// ======= 공격 카드 효과 타입 =======
enum class AtkEffectType
{
    NormalDamage,        
    TrueDamage,          
    FollowUpMultiplier,  
    CoinTossDamage,      
    OnlyIfEnemyDefended, 
    SplitWithStatus      
};

struct AtkEffectParam
{
    int baseDamage = 0;
    float multiplier = 0.0f;
    StatusType statusType = StatusType::None;
    int statusTurns = 0;
    int statusDamagePerTurn = 0;
};

class AtkCard : public Card
{
private:
    AtkEffectType m_effect = AtkEffectType::NormalDamage;
    AtkEffectParam m_param;

public:
    AtkCard(CAttribute attr, CRank rank, AtkEffectType effect);

    // 카드 효과 적용(WinAPI/게임로직에서 호출)
    DamageResult Resolve(const BattleContext& ctx, std::mt19937& rng) const;

    // ===== 편의 생성 함수 =====
    static AtkCard MakeNormal(CAttribute a, CRank r);
    static AtkCard MakeTrueDamage(CAttribute a, CRank r);
    static AtkCard MakeFollowUp(CAttribute a, CRank r);
    static AtkCard MakeCoinToss(CAttribute a, CRank r);
    static AtkCard MakeOnlyIfEnemyDefended(CAttribute a, CRank r);
    static AtkCard MakeSplitWithPoison(CAttribute a, CRank r, int turns = 3);
    static AtkCard MakeSplitWithBleed(CAttribute a, CRank r, int turns = 3);

private:
    static int RankTo_5_10_15(CRank r);
    static int RankTo_1_2_3(CRank r);
    static float RankTo_0_25_0_5_0_75(CRank r);
};
