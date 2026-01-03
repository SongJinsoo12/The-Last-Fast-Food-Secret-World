#pragma once
#include <vector>

// AtkCard 계산에 필요한 "전투 문맥"
struct BattleContext
{
    int currentTurn = 1;

    // 이번 턴 상대가 방어카드를 사용했는지(조건 카드)
    bool enemyUsedDefenseThisTurn = false;

    // 직전 공격으로 실제로 준 피해(연계 카드)
    int lastAttackDamageDealt = 0;

    // 공격 직후 연계 카드 사용 가능 상태
    bool canUseFollowUp = false;
};

enum class StatusType { None, Poison, Bleed };

struct StatusEffect
{
    StatusType type = StatusType::None;
    int remainingTurns = 0;
    int damagePerTurn = 0;
};

struct DamageResult
{
    int immediateDamage = 0;
    bool isTrueDamage = false;   // 방어 무시(원하면)
    bool addStatus = false;
    StatusEffect status;
};
