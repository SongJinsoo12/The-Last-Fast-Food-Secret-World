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
    void Same_Card(Player& player);                          // 최근 사용한 2장의 카드가 같으면 2장 드로우
    void Card_Draw_Enemy_Heal(Player& player, Mob& mob);     // 1장 드로우 상대 회복
    void Card_Draw_CockroachCard(Player& player);            // 3장 드로우 후 바퀴벌레 카드 패에 추가
    void Hp_Down_Card_Draw(Player& player);                  // 자신의 체력 감소 1장 드로우
    void Card_Draw_Damage_Doun(Player& player, float dmg = 0.5f); // 2장 드로우 데미지 1/2배로 변경
    void Card_Draw_Disarray(Player& player);                 // 2장 드로우, 그후 1턴 동안 혼란
    void Card_Forsake_Heal_Atk_Change(Player& player, Mob& enemy);       // 자신의 패 1장을 버림 그 후 1턴 동안 회복=>데미지
    void TwoCard_Get_Enemy_Card_Get(Player& player, Mob& mob); // 카드 2장 상대 1장 획득

    // --- 공격 강화/변형 ---
    void Next_AtkCard_Damage_Up(Player& player, float mult = 1.5f);  // 다음 공격 피해량 배율
    void MY_Attiravate_Change(CAttribute ait);                       // TODO: 다음 공격 속성 변경(추가효과는 해석기 필요)
    void Card_Forsake_Damage_up(Player& player, float dmg = 2.0f);   // 자신의 패 2장을 버린 후 공격 카드의 피해를 2배로 변경
    void Atk_Or_Def(Player* player, Mob* mob);                       // 전 턴에 상대가 마지막으로 사용한 카드에 따라 다음 카드 변경

    // --- 턴/플레이 제한 ---
    void Three_Turn_After_Three_Card(Player& player);         // 3턴 후 3장 사용 가능
    void Used_TwoCard_But_Drow_Prohibition(Player& player);   // 지금 턴 +2장 더 사용, 다음 2턴 드로우 금지
    void AtkCard_Forsake_Used_TwoCard(Player& player, CardManager& cm);        // 공격 카드 1장 버림 그후 2장 사용 가능
    void Instant_Turn_Card(Player& player);                   // 카드 즉시 발동

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
