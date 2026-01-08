#pragma once

#include "Card.h"
#include "BattleTypes.h" // BattleContext

#include <string>
#include <random>

// ================================
//  DefCard (방어 카드)
//  - 조건 목록(요청사항)
//    1) 방어 = 5/10/15 (20/40/60)%
//    2) 피해 방어(고정 감소) 5/10/15
//    3) 상성 속성을 제외한 나머지 속성 공격 방어(단, 무속성은 방어x)
//    4) 무속성 피해 감소 20/40/60%
//    5) 무적 방어 = 1회 무적 + 1장 버림
//    6) 데미지 반사 20/40/60%
//
//  설계 메모
//  - DefCard는 "사용하면 DefenseBuff"를 생성합니다.
//  - DefenseBuff는 1회(혹은 1턴) 동안 유지되는 방어 효과의 묶음이며,
//    실제 공격 적용(데미지 감소/반사/무적 소비)은 ApplyDefenseToHit()로 계산합니다.
//  - 프로젝트의 전투 흐름(손패/버림 등)에 맞게, 반환값의 discardOnUse를 활용하세요.
// ================================

enum class DefKind
{
    GuardPercentAndShield,   // 방어 = 5/10/15 (20/40/60)%
    FlatBlock,               // 피해 방어(고정) 5/10/15
    ElementalBarrier,        // 상성 속성을 제외한 나머지 속성 방어(무속성 제외)
    NeutralReduction,        // 무속성 피해 감소 20/40/60%
    InvincibleOnce,          // 1회 무적 + 1장 버림
    ReflectPercent           // 데미지 % 반사
};

// 방어 효과(버프)
struct DefenseBuff
{
    int remainingTurns = 1;   // 기본 1턴(필요시 변경)

    // 기본 방어 수치
    int shieldGain = 0;       // 실드(피해 흡수용)
    int percentReduce = 0;    // 전체 피해 %감소(0~100)
    int flatReduce = 0;       // 피해 고정 감소

    // 속성 조건 방어
    bool blockNonCounterElements = false; // "상성 속성"을 제외하고 방어
    int neutralPercentReduce = 0;         // 무속성(E_BREAD) 피해 %감소

    // 특수
    int invincibleCharges = 0; // 1이면 다음 1회 공격 무적
    int discardOnUse = 0;      // 사용 시 버릴 카드 수(손패 시스템 연동)
    int reflectPercent = 0;    // 받은 피해의 %를 공격자에게 반사
};

// 방어 적용 결과(단일 피격)
struct DefenseHitResult
{
    int finalDamageToDefender = 0; // 방어 적용 후 방어자가 실제로 받는 피해
    int blockedByShield = 0;       // 실드로 막은 양
    int reducedByBuff = 0;         // 버프로 감소된 양(퍼센트/고정 포함)
    int reflectedToAttacker = 0;   // 반사 피해
    bool consumedInvincible = false;
};

// 상성 규칙(프로젝트 규칙이 없다면 이 기본 순환을 사용)
// Bulgogi > Source > Chesse > Vegat > Bulgogi
// Bread는 무속성
bool IsCounterAttribute(CAttribute defender, CAttribute attacker);

// 단일 공격(한 번의 히트)에 방어 버프를 적용
// - shield는 피해를 흡수하며, 막은 양만큼 감소합니다.
// - trueDamage면 percent/flat/속성방어는 무시하지만(원하면),
//   invincible은 적용되도록 처리했습니다.
DefenseHitResult ApplyDefenseToHit(
    int incomingDamage,
    CAttribute attackAttr,
    bool isTrueDamage,
    CAttribute defenderAttr,
    DefenseBuff& buff,
    int& defenderShield /*in/out*/
);

class DefCard : public Card
{
private:
    std::wstring name;
    DefKind kind;

public:
    DefCard(DefKind kind, CRank rank, CAttribute attr, const std::wstring& name);
    virtual ~DefCard() = default;

    const std::wstring& GetName() const { return name; }
    DefKind GetKind() const { return kind; }
    CRank GetRank() const { return Rank; }
    CAttribute GetAttribute() const { return Ait; }

    // 카드 사용 시 방어 버프 생성
    // (랜덤이 필요하면 rng/ctx를 활용할 수 있도록 시그니처를 맞춤)
    virtual DefenseBuff Resolve(const BattleContext& ctx, std::mt19937& rng, CAttribute ownerAttr) const;
};

// 편의: 종류별 기본 카드 생성
// (프로젝트에서는 덱 구성 시 아래 함수를 사용해도 됩니다.)
DefCard MakeGuardCard(CRank rank, CAttribute attr);
DefCard MakeFlatBlockCard(CRank rank, CAttribute attr);
DefCard MakeElementalBarrierCard(CRank rank, CAttribute attr);
DefCard MakeNeutralReductionCard(CRank rank, CAttribute attr);
DefCard MakeInvincibleCard(CRank rank, CAttribute attr);
DefCard MakeReflectCard(CRank rank, CAttribute attr);
