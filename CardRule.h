#pragma once
#include "Card.h"
#include <vector>

// 상성 끼리의 대미지 체감
// 식중독 상태이상
// 플레이어의 행동 저장
// 상대 해동 저장

class CardRule
{
public:
    bool Status; // 독이 있으면 1, 독이없으면 0

public:
    CardRule();
    virtual ~CardRule() = default;

    // 공격이랑 방어쪽 기본 데미지 및 실드량
    static int RankValue(CRank rank);
    // 계급끼리의 대미지
    static int RankMatchUp(CRank Atk, CRank Def);
    // 상성끼리의 대미지
    static bool IsCounterAttribute(CAttribute Atk, CAttribute Def);;
};