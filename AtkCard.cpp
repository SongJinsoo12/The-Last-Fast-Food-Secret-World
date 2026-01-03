#include "AtkCard.h"
#include <cmath>

int AtkCard::RankTo_5_10_15(CRank r)
{
    switch (r)
    {
    case Star_1: return 5;
    case Star_2: return 10;
    case Star_3: return 15;
    default: return 5;
    }
}

int AtkCard::RankTo_1_2_3(CRank r)
{
    switch (r)
    {
    case Star_1: return 1;
    case Star_2: return 2;
    case Star_3: return 3;
    default: return 1;
    }
}

float AtkCard::RankTo_0_25_0_5_0_75(CRank r)
{
    switch (r)
    {
    case Star_1: return 0.25f;
    case Star_2: return 0.50f;
    case Star_3: return 0.75f;
    default: return 0.25f;
    }
}

AtkCard::AtkCard(CAttribute attr, CRank rank, AtkEffectType effect)
{
    // Card 베이스 멤버 초기화
    Type = E_Attack;
    Ait = attr;
    Rank = rank;

    m_effect = effect;

    // 효과별 파라미터 세팅(요구사항 반영)
    if (effect == AtkEffectType::NormalDamage)
    {
        m_param.baseDamage = RankTo_5_10_15(rank);
    }
    else if (effect == AtkEffectType::TrueDamage)
    {
        m_param.baseDamage = RankTo_5_10_15(rank);
    }
    else if (effect == AtkEffectType::FollowUpMultiplier)
    {
        m_param.multiplier = RankTo_0_25_0_5_0_75(rank);
    }
    else if (effect == AtkEffectType::CoinTossDamage)
    {
        m_param.baseDamage = RankTo_5_10_15(rank);
    }
    else if (effect == AtkEffectType::OnlyIfEnemyDefended)
    {
        m_param.baseDamage = RankTo_5_10_15(rank);
    }
    else if (effect == AtkEffectType::SplitWithStatus)
    {
        m_param.baseDamage = RankTo_5_10_15(rank);
        m_param.statusDamagePerTurn = RankTo_1_2_3(rank);
        m_param.statusTurns = 3;
        m_param.statusType = StatusType::Poison; // 기본(팩토리에서 변경)
    }

    // 호환: 베이스 Card의 Atk 필드도 즉시피해로 맞춰둠(필요 없으면 제거)
    Atk = m_param.baseDamage;
    Rdc = 0;
}

DamageResult AtkCard::Resolve(const BattleContext& ctx, std::mt19937& rng) const
{
    DamageResult out{};

    switch (m_effect)
    {
    case AtkEffectType::NormalDamage:
        out.immediateDamage = m_param.baseDamage;
        break;

    case AtkEffectType::TrueDamage:
        out.immediateDamage = m_param.baseDamage;
        out.isTrueDamage = true;
        break;

    case AtkEffectType::FollowUpMultiplier:
        // 공격 직후만 가능 + 직전 데미지가 있어야 의미 있음
        if (ctx.canUseFollowUp && ctx.lastAttackDamageDealt > 0)
            out.immediateDamage = (int)std::floor(ctx.lastAttackDamageDealt * m_param.multiplier);
        else
            out.immediateDamage = 0;
        break;

    case AtkEffectType::CoinTossDamage:
    {
        std::uniform_int_distribution<int> dist(0, 1);
        const bool success = (dist(rng) == 1);
        out.immediateDamage = success ? m_param.baseDamage : 0;
        break;
    }

    case AtkEffectType::OnlyIfEnemyDefended:
        out.immediateDamage = ctx.enemyUsedDefenseThisTurn ? m_param.baseDamage : 0;
        break;

    case AtkEffectType::SplitWithStatus:
        out.immediateDamage = m_param.baseDamage;
        out.addStatus = true;
        out.status.type = m_param.statusType;
        out.status.remainingTurns = m_param.statusTurns;
        out.status.damagePerTurn = m_param.statusDamagePerTurn;
        break;
    }

    return out;
}

// ===== 편의 생성 함수 =====
AtkCard AtkCard::MakeNormal(CAttribute a, CRank r)
{ 
    return AtkCard(a, r, AtkEffectType::NormalDamage); 
}
AtkCard AtkCard::MakeTrueDamage(CAttribute a, CRank r) 
{
    return AtkCard(a, r, AtkEffectType::TrueDamage);
}
AtkCard AtkCard::MakeFollowUp(CAttribute a, CRank r) 
{ 
    return AtkCard(a, r, AtkEffectType::FollowUpMultiplier); 
}
AtkCard AtkCard::MakeCoinToss(CAttribute a, CRank r) 
{ 
    return AtkCard(a, r, AtkEffectType::CoinTossDamage);
}
AtkCard AtkCard::MakeOnlyIfEnemyDefended(CAttribute a, CRank r)
{ 
    return AtkCard(a, r, AtkEffectType::OnlyIfEnemyDefended);
}

AtkCard AtkCard::MakeSplitWithPoison(CAttribute a, CRank r, int turns)
{
    AtkCard c(a, r, AtkEffectType::SplitWithStatus);
    c.m_param.statusType = StatusType::Poison;
    c.m_param.statusTurns = turns;
    return c;
}

AtkCard AtkCard::MakeSplitWithBleed(CAttribute a, CRank r, int turns)
{
    AtkCard c(a, r, AtkEffectType::SplitWithStatus);
    c.m_param.statusType = StatusType::Bleed;
    c.m_param.statusTurns = turns;
    return c;
}
