#pragma once

#include "Card.h"   // CAttribute, CRank, CardManager

class Player;
class Mob;
class AtkCard;

// SupportCard (patched)
// - Player/Mob의 덱/손패가 vector<int> cardId 형태라는 가정(현재 Player.cpp/Mob.cpp와 동일)
// - CardManager는 UI용(임시)이라, 실제 효과는 Player/Mob API로 처리하는 방향

class SupportCard
{
public:
    SupportCard();

    // --- 기본 ---
    void HealCard(Player& player);                          // 회복
    void Heal_Turn_Three_Hp_Down(Player& player);            // 회복 후 3턴 뒤 (heal+25%) 체력 감소

    // --- 상태이상 ---
    void ApplyPoison(int turns, int dmgPerTurn);             // 독 부여(내부 상태)
    void TickPoison_BeginTurn(Player& player);               // 턴 시작 시 독 틱
    void RemoveStatus(Player& player);                       // 상태 이상 즉시 제거(현재는 독만)

    // --- 카드 조작 ---
    void GetCard(Player& player, Mob& enemy);                // 상대 덱에서 1장 가져오기
    void Card_Forsake_Card_Draw(Player& player);             // 패 1장 버린 후 1장 뽑기

    // --- 공격 강화/변형 ---
    void Next_AtkCard_Damage_Up(Player& player, float mult = 1.5f);  // 다음 공격 피해량 배율
    void MY_Attiravate_Change(CAttribute ait);                        // TODO: 다음 공격 속성 변경(추가효과는 해석기 필요)

    // --- 턴/플레이 제한 ---
    void Three_Turn_After_Three_Card(Player& player);         // 3턴 후 3장 사용 가능
    void Used_TwoCard_But_Drow_Prohibition(Player& player);   // 지금 턴 +2장 더 사용, 다음 2턴 드로우 금지

    // --- 지연 회복 ---
    void Two_Turn_Heal(Player& player);                      // 2턴 후 체력 heal 회복

    // --- 상대/패 훔치기 ---
    void MY_Two_Card_Forsake_Get_Card(Player& player, Mob& enemy);    // 내 패 2장 버리고 상대 패 1장 가져오기

    // --- 덱 상단 보기(간단 버전) ---
    void SeeCard(Player& player);                             // 덱 맨 위 3장 보고(현재는 reverse로 재정렬)

    // --- 상대 제어(간단 버전) ---
    void Enemy_Atk_Prohibition(Mob& mob);                     // 상대 '플레이' 금지 1턴(공격만 금지는 분리 구현 필요)
    void Enemy_Forsake_Card(Mob& enemy);                      // 상대 패에서 1장 강제 버리기(무작위)

    // --- 양쪽 패를 덱으로 되돌리고 N장씩 드로우 ---
    void Deck_Retrun_Card(Player& player, Mob& enemy, int drawEach);

public:
    int heal = 10;

private:
    void PoisonDelete(Player& player);

private:
    bool m_poisonActive = false;
    int  m_poisonTurnsLeft = 0;
    int  m_poisonDmgPerTurn = 0;
};
