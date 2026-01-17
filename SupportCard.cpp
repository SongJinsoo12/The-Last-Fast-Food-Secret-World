#include "SupportCard.h"

#include "Player.h"
#include "Mob.h"
#include "AtkCard.h"

#include <algorithm>

SupportCard::SupportCard()
{
    heal = 10;
}

void SupportCard::HealCard(Player& player)
{
    player.Heal(heal);
}

void SupportCard::Heal_Turn_Three_Hp_Down(Player& player)
{
    // 즉시 회복
    player.Heal(heal);

    // 3턴 뒤: (heal + 25%) 만큼 체력 감소
    // (Player::AddDelayedHpLoss 내부에서 1.25 배 적용)
    player.AddDelayedHpLoss(3, heal);
}

void SupportCard::ApplyPoison(int turns, int dmgPerTurn)
{
    if (turns <= 0 || dmgPerTurn <= 0)
    {
        m_poisonActive = false;
        m_poisonTurnsLeft = 0;
        m_poisonDmgPerTurn = 0;
        return;
    }

    m_poisonActive = true;
    m_poisonTurnsLeft = turns;
    m_poisonDmgPerTurn = dmgPerTurn;
}

void SupportCard::PoisonDelete(Player& player)
{
    m_poisonActive = false;
    m_poisonTurnsLeft = 0;
    m_poisonDmgPerTurn = 0;
    player.SetPoison(false);
}

void SupportCard::TickPoison_BeginTurn(Player& player)
{
    if (!player.HasPoison())
        return;

    if (!m_poisonActive)
    {
        player.SetPoison(false);
        return;
    }

    if (m_poisonTurnsLeft <= 0)
    {
        PoisonDelete(player);
        return;
    }

    player.Damage(m_poisonDmgPerTurn);
    --m_poisonTurnsLeft;

    if (m_poisonTurnsLeft <= 0)
        PoisonDelete(player);
}

void SupportCard::RemoveStatus(Player& player)
{
    // 현재 프로젝트에서 즉시 제거가 필요한 상태가 '독'만 있다고 가정
    if (player.HasPoison())
        PoisonDelete(player);
}

void SupportCard::GetCard(Player& player, Mob& enemy)
{
    int stolenId = -1;
    if (enemy.PopTopDeckCard(stolenId))
    {
        player.AddCardToHand(stolenId);
        return;
    }

    // 상대 덱도, 버림더미도 비어서 못 가져오는 경우: 아무일도 없음
}

void SupportCard::Card_Forsake_Card_Draw(Player& player)
{
    // "카드 1장을 버린 후 다시 1장 뽑기" (추가 사용횟수 차감 없음)
    // - 선택 카드가 있으면 그걸 버리고, 없으면 무작위 1장 버림
    const int before = player.GetHandSize();
    if (before <= 0) return;

    // 선택 카드 버리기 시도
    if (!player.DiscardHandCardAt(player.ChooseCardIndex()))
    {
        player.DiscardRandomHandCards(1);
    }

    player.DrawCards(1);
}

void SupportCard::Next_AtkCard_Damage_Up(Player& player, float mult)
{
    if (mult <= 0.0f) mult = 1.0f;
    player.SetNextAtkMultiplier(mult);
}

void SupportCard::MY_Attiravate_Change(CAttribute ait)
{
    // TODO: "다음 공격 카드의 속성 변경"은 카드 해석기/리졸버 단계에서 적용되어야 안전합니다.
    (void)ait;
}

void SupportCard::Three_Turn_After_Three_Card(Player& player)
{
    // 3턴 후 해당 턴에 3장 사용 가능
    player.SchedulePlayLimit(3, 3);
}

void SupportCard::Used_TwoCard_But_Drow_Prohibition(Player& player)
{
    // 지금 턴에 2장 더 사용 가능 (기본 1장 + 2장 = 3장)
    player.AddExtraPlaysThisTurn(2);

    // 다음 2턴 동안 일반 드로우 불가
    player.AddNoDrawTurns(2);
}

void SupportCard::Two_Turn_Heal(Player& player)
{
    player.AddDelayedHeal(2, heal);
}

void SupportCard::MY_Two_Card_Forsake_Get_Card(Player& player, Mob& enemy)
{
    // 내 패 2장 버리고
    if (!player.DiscardRandomHandCards(2))
        return;

    // 상대 패에서 무작위 1장 가져오기
    int stolen = -1;
    if (enemy.PopRandomHandCard(stolen))
        player.AddCardToHand(stolen);
}

void SupportCard::SeeCard(Player& player)
{
    // "덱 맨 위 3장을 보고 순서 변경"은 UI 선택이 필요하지만,
    // 여기서는 구현 예시로 "상단 3장 reverse"만 제공합니다.
    std::vector<int> top;
    if (!player.PeekTopDeck(3, top))
        return;

    std::reverse(top.begin(), top.end());
    player.ReorderTopDeck(top);
}

void SupportCard::Enemy_Atk_Prohibition(Mob& mob)
{
    // 공격 카드만 금지하려면 cardId -> 타입 해석이 필요합니다.
    // 간단 버전: "다음 1턴동안 어떤 카드도 못 냄"
    mob.ProhibitPlay(1);
}

void SupportCard::Enemy_Forsake_Card(Mob& enemy)
{
    enemy.DiscardRandomHandCards(1);
}

void SupportCard::Deck_Retrun_Card(Player& player, Mob& enemy, int drawEach)
{
    if (drawEach < 0) drawEach = 0;

    // 양쪽 손패를 덱으로 되돌림
    player.ReturnHandToDeck(true);
    enemy.ReturnHandToDeck(true);

    // 각자 drawEach 만큼 드로우
    player.DrawCards(drawEach);
    enemy.DrawCards(drawEach);
}
