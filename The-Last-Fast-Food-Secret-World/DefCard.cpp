#include "DefCard.h"

#include <algorithm>

// ================================
// 속성 상성(기본)
//  Bulgogi > Source > Chesse > Vegat > Bulgogi
//  Bread(E_BREAD)는 무속성(상성 계산 제외)
//
//  ※ 프로젝트의 실제 상성 규칙이 다르면, 이 함수만 바꾸면 됩니다.
// ================================
bool IsCounterAttribute(CAttribute defender, CAttribute attacker)
{
    if (defender == E_BREAD || attacker == E_BREAD) return false;

    switch (defender)
    {
    case E_BULGOGI: return attacker == E_VEGAT;  // Vegat가 Bulgogi 상성(카운터)
    case E_SOURCE:  return attacker == E_BULGOGI;
    case E_CHESSE:  return attacker == E_SOURCE;
    case E_VEGAT:   return attacker == E_CHESSE;
    default:        return false;
    }
}

static int RankValue(CRank r, int v1, int v2, int v3)
{
    switch (r)
    {
    case Star_1: return v1;
    case Star_2: return v2;
    case Star_3: return v3;
    default:     return v1;
    }
}

DefenseHitResult ApplyDefenseToHit(
    int incomingDamage,
    CAttribute attackAttr,
    bool isTrueDamage,
    CAttribute defenderAttr,
    DefenseBuff& buff,
    int& defenderShield
)
{
    DefenseHitResult out;
    if (incomingDamage < 0) incomingDamage = 0;

    // 1) 무적(1회)
    if (buff.invincibleCharges > 0)
    {
        buff.invincibleCharges -= 1;
        out.consumedInvincible = true;
        out.finalDamageToDefender = 0;
        return out;
    }

    int dmg = incomingDamage;

    // 2) 버프 기반 감소(트루데미지면 보통 무시)
    if (!isTrueDamage)
    {
        int reduce = 0;

        // (a) 속성 조건: 무속성 감소
        if (attackAttr == E_BREAD && buff.neutralPercentReduce > 0)
        {
            reduce += (dmg * buff.neutralPercentReduce) / 100;
        }

        // (b) 속성 조건: 상성 속성 제외 방어(무속성은 방어x)
        // - attacker가 무속성이면 적용 X
        // - attacker가 defender의 상성이면 적용 X
        // - 그 외 속성 공격이면 percentReduce 만큼 감소
        if (buff.blockNonCounterElements)
        {
            const bool attackerIsNeutral = (attackAttr == E_BREAD);
            const bool attackerIsCounter = IsCounterAttribute(defenderAttr, attackAttr);

            if (!attackerIsNeutral && !attackerIsCounter)
            {
                reduce += (dmg * buff.percentReduce) / 100;
            }
        }
        else
        {
            // 일반 퍼센트 감소
            if (buff.percentReduce > 0)
                reduce += (dmg * buff.percentReduce) / 100;
        }

        // (c) 고정 감소
        reduce += buff.flatReduce;

        // clamp
        reduce = std::clamp(reduce, 0, dmg);
        dmg -= reduce;
        out.reducedByBuff = reduce;
    }

    // 3) 실드로 흡수
    if (defenderShield > 0 && dmg > 0)
    {
        int blocked = std::min(defenderShield, dmg);
        defenderShield -= blocked;
        dmg -= blocked;
        out.blockedByShield = blocked;
    }

    if (dmg < 0) dmg = 0;
    out.finalDamageToDefender = dmg;

    // 4) 반사(실제로 받는 피해 기준)
    if (buff.reflectPercent > 0 && out.finalDamageToDefender > 0)
    {
        out.reflectedToAttacker = (out.finalDamageToDefender * buff.reflectPercent) / 100;
    }

    return out;
}

DefCard::DefCard(DefKind kind, CRank rank, CAttribute attr, const std::wstring& name)
    : name(name), kind(kind)
{
    // Card 기본 필드 세팅
    m_Type = E_Defence;
    Rank = rank;
    m_Ait = attr;
    Atk = 0;
    Rdc = 0;
}

DefenseBuff DefCard::Resolve(const BattleContext& /*ctx*/, std::mt19937& /*rng*/, CAttribute /*ownerAttr*/) const
{
    DefenseBuff b;
    b.remainingTurns = 1;

    switch (kind)
    {
    case DefKind::GuardPercentAndShield:
        // 방어 = 5/10/15 (20/40/60)%
        b.shieldGain = RankValue(Rank, 5, 10, 15);
        b.percentReduce = RankValue(Rank, 20, 40, 60);
        break;

    case DefKind::FlatBlock:
        // 피해 방어(고정 감소) 5/10/15
        b.flatReduce = RankValue(Rank, 5, 10, 15);
        break;

    case DefKind::ElementalBarrier:
        // 상성 속성을 제외한 나머지 속성 공격 방어(단, 무속성은 방어x)
        // - 프로젝트에 따라 '방어량'을 조절: 여기서는 20/40/60% 감소로 처리
        b.blockNonCounterElements = true;
        b.percentReduce = RankValue(Rank, 20, 40, 60);
        break;

    case DefKind::NeutralReduction:
        // 무속성 피해 감소 20/40/60%
        b.neutralPercentReduce = RankValue(Rank, 20, 40, 60);
        break;

    case DefKind::InvincibleOnce:
        // 무적 방어 = 1회 무적 + 1장 버림
        b.invincibleCharges = 1;
        b.discardOnUse = 1;
        break;

    case DefKind::ReflectPercent:
        // 데미지 20/40/60% 반사
        b.reflectPercent = RankValue(Rank, 20, 40, 60);
        break;
    }

    return b;
}

// ====== Factory helpers ======
DefCard MakeGuardCard(CRank rank, CAttribute attr)
{
    return DefCard(DefKind::GuardPercentAndShield, rank, attr, L"가드");
}

DefCard MakeFlatBlockCard(CRank rank, CAttribute attr)
{
    return DefCard(DefKind::FlatBlock, rank, attr, L"블록");
}

DefCard MakeElementalBarrierCard(CRank rank, CAttribute attr)
{
    return DefCard(DefKind::ElementalBarrier, rank, attr, L"속성 장벽");
}

DefCard MakeNeutralReductionCard(CRank rank, CAttribute attr)
{
    return DefCard(DefKind::NeutralReduction, rank, attr, L"무속성 저항");
}

DefCard MakeInvincibleCard(CRank rank, CAttribute attr)
{
    // 랭크는 의미 없지만(항상 1회 무적), 통일성을 위해 받습니다.
    return DefCard(DefKind::InvincibleOnce, rank, attr, L"무적 방어");
}

DefCard MakeReflectCard(CRank rank, CAttribute attr)
{
    return DefCard(DefKind::ReflectPercent, rank, attr, L"반사");
}
