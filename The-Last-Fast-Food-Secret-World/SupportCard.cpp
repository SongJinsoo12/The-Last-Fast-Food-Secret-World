#include "SupportCard.h"
#include "Player.h"
#include "Mob.h"
#include "AtkCard.h"

#include <algorithm>

// =====================
// SupportCard 내부 함수 실행 여부 디버그 로그
// - 각 SupportCard 메서드 진입/종료 + 주요 내부 호출을 찍어서
//   "SupportCard는 불렸는데, 그 안의 함수(드로우/버리기/상태변경)가 실제로 실행됐는지"를 확인할 수 있게 함.
// - 로그가 너무 많으면 SUPPORT_TRACE_ENABLED 를 0 으로 바꾸세요.
// =====================
#ifndef SUPPORT_TRACE_ENABLED
#define SUPPORT_TRACE_ENABLED 1
#endif

#if SUPPORT_TRACE_ENABLED
static void _SC_DumpState(const char* tag, const Player* p, const Mob* m)
{
    if (!tag) tag = "";
    std::cout << "[SC:STATE] " << tag;
    if (p)
    {
        std::cout << " | P(Hp=" << p->GetHP() << "/" << p->GetMaxHP()
                  << ",Hand=" << p->GetHandSize()
                  << ",Deck=" << p->GetDeckSize()
                  << ",Discard=" << p->GetDiscardSize();
        // 자주 쓰는 상태들
        std::cout << ",Poison=" << (p->HasPoison() ? 1 : 0) << ")";
    }
    if (m)
    {
        std::cout << " | M(Hp=" << m->GetHP() << "/" << m->GetMaxHP()
                  << ",Hand=" << m->GetHandSize()
                  << ",Deck=" << m->GetDeckSize()
                  << ",Discard=" << m->GetDiscardSize() << ")";
    }
    std::cout << "\n";
}

static void _SC_Log(const char* msg)
{
    if (!msg) msg = "";
    std::cout << msg << "\n";
}

#define SC_BEGIN(name, pPtr, mPtr) do { std::cout << "[SC:BEGIN] " << (name) << "\n"; _SC_DumpState("BEFORE", (pPtr), (mPtr)); } while(0)
#define SC_END(name, pPtr, mPtr)   do { _SC_DumpState("AFTER ", (pPtr), (mPtr)); std::cout << "[SC:END] " << (name) << "\n"; } while(0)
#define SC_STEP(label)       do { std::cout << "  [SC:STEP] " << (label) << "\n"; } while(0)
#define SC_SKIP(reason)      do { std::cout << "  [SC:SKIP] " << (reason) << "\n"; } while(0)
#else
#define SC_BEGIN(name, pPtr, mPtr) do {} while(0)
#define SC_END(name, pPtr, mPtr)   do {} while(0)
#define SC_STEP(label)       do {} while(0)
#define SC_SKIP(reason)      do {} while(0)
#endif

SupportCard::SupportCard()
{
    heal = 10;
}

void SupportCard::HealCard(Player& player)
{
    SC_BEGIN("HealCard", &player, nullptr);
    SC_STEP("player.Heal(heal)");
    player.Heal(heal);
    cout << "힐을 했습니다." << endl;
    SC_END("HealCard", &player, nullptr);
}

void SupportCard::Heal_Turn_Three_Hp_Down(Player& player)
{
    SC_BEGIN("Heal_Turn_Three_Hp_Down", &player, nullptr);
    // 즉시 회복
    SC_STEP("player.Heal(heal)");
    player.Heal(heal);

    // 3턴 뒤: (heal + 25%) 만큼 체력 감소
    // (Player::AddDelayedHpLoss 내부에서 1.25 배 적용)
    SC_STEP("player.AddDelayedHpLoss(3, heal)");
    player.AddDelayedHpLoss(3, heal);
    cout << "3턴뒤 데미지를 입습니다." << endl;
    SC_END("Heal_Turn_Three_Hp_Down", &player, nullptr);
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
    cout << "독이 없다!!" << endl;
}

void SupportCard::TickPoison_BeginTurn(Player& player)
{
    SC_BEGIN("TickPoison_BeginTurn", &player, nullptr);
    if (!player.HasPoison())
    {
        SC_SKIP("player.HasPoison()==false");
        SC_END("TickPoison_BeginTurn", &player, nullptr);
        return;
    }

    if (!m_poisonActive)
    {
        player.SetPoison(false);
        SC_SKIP("m_poisonActive==false -> player.SetPoison(false)");
        SC_END("TickPoison_BeginTurn", &player, nullptr);
        return;
    }

    if (m_poisonTurnsLeft <= 0)
    {
        PoisonDelete(player);
        SC_SKIP("m_poisonTurnsLeft<=0 -> PoisonDelete");
        SC_END("TickPoison_BeginTurn", &player, nullptr);
        return;
    }

    SC_STEP("player.Damage(m_poisonDmgPerTurn)");
    player.Damage(m_poisonDmgPerTurn);
    --m_poisonTurnsLeft;

    if (m_poisonTurnsLeft <= 0)
        PoisonDelete(player);

    SC_END("TickPoison_BeginTurn", &player, nullptr);
}

void SupportCard::RemoveStatus(Player& player)
{
    SC_BEGIN("RemoveStatus", &player, nullptr);
    // 현재 프로젝트에서 즉시 제거가 필요한 상태가 '독'만 있다고 가정
    if (player.HasPoison())
    {
        SC_STEP("PoisonDelete(player)");
        PoisonDelete(player);
    }
    else
    {
        SC_SKIP("no status to remove (poison=false)");
    }
    SC_END("RemoveStatus", &player, nullptr);
}

void SupportCard::GetCard(Player& player, Mob& enemy)
{
    SC_BEGIN("GetCard", &player, &enemy);
    int stolenId = -1;
    SC_STEP("enemy.PopTopDeckCard(stolenId)");
    if (enemy.PopTopDeckCard(stolenId))
    {
        SC_STEP("player.AddCardToHand(stolenId)");
        player.AddCardToHand(stolenId);
        cout << "니껀 내꺼 내껀 내꺼 알았냐?" << endl;
        SC_END("GetCard", &player, &enemy);
        return;
    }

    // 상대 덱도, 버림더미도 비어서 못 가져오는 경우: 아무일도 없음
    SC_SKIP("enemy deck empty -> PopTopDeckCard failed");
    SC_END("GetCard", &player, &enemy);
}

void SupportCard::Card_Forsake_Card_Draw(Player& player)
{
    SC_BEGIN("Card_Forsake_Card_Draw", &player, nullptr);
    // "카드 1장을 버린 후 다시 1장 뽑기" (추가 사용횟수 차감 없음)
    // - 선택 카드가 있으면 그걸 버리고, 없으면 무작위 1장 버림
    const int before = player.GetHandSize();
    if (before <= 0)
    {
        SC_SKIP("hand empty");
        SC_END("Card_Forsake_Card_Draw", &player, nullptr);
        return;
    }

    // 선택 카드 버리기 시도
    SC_STEP("player.DiscardHandCardAt(player.ChooseCardIndex())");
    if (!player.DiscardHandCardAt(player.ChooseCardIndex()))
    {
        SC_STEP("fallback: player.DiscardRandomHandCards(1)");
        player.DiscardRandomHandCards(1);
        cout << "한장 버린다" << endl;
    }
    SC_STEP("player.DrawCards(1)");
    player.DrawCards(1);
    cout << "1장을 뽑는다." << endl;
    SC_END("Card_Forsake_Card_Draw", &player, nullptr);
}

void SupportCard::Same_Card(Player& player)
{
    SC_BEGIN("Same_Card", &player, nullptr);
    // If the most recent two non-support cards (Attack/Defence) that were
    // discarded or played are the same type -> draw 2.
    SC_STEP("player.HasRecentTwoSameCombatType() check");
    if (player.HasRecentTwoSameCombatType())
    {
        SC_STEP("player.DrawCards(2)");
        player.DrawCards(2);
        cout << "2장 드로우 됐냐?" << endl;
    }
    else
    {
        SC_SKIP("recent-two-same-combat-type == false");
    }
    SC_END("Same_Card", &player, nullptr);
}


CardManager cm;
Card cd;
void SupportCard::Card_Draw_Enemy_Heal(Player& player, Mob& mob)
{
    SC_BEGIN("Card_Draw_Enemy_Heal", &player, &mob);
    // 카드 1장 드로우
    SC_STEP("player.DrawCards(1)");
    player.DrawCards(1);
    //player.DrawCard(1);
    SC_STEP("mob.Heal(10)");
    mob.Heal(10);
    cout << "드로우 그리고 넌 까까 먹어" << endl;
    SC_END("Card_Draw_Enemy_Heal", &player, &mob);
}

void SupportCard::Card_Draw_CockroachCard(Player& player)
{
    SC_BEGIN("Card_Draw_CockroachCard", &player, nullptr);
    SC_STEP("player.DrawCards(3)");
    player.DrawCards(3);
    SC_STEP("player.AddCardToDeck(Cockroach, true)");
    player.AddCardToDeck((int)CardId::Cockroach, true);
    cout << "난 3장 드로우 그리고 바퀴벌레을 꺼내겠다." << endl;
    SC_END("Card_Draw_CockroachCard", &player, nullptr);
}

void SupportCard::Hp_Down_Card_Draw(Player& player)
{
    SC_BEGIN("Hp_Down_Card_Draw", &player, nullptr);
    SC_STEP("player.Heal(-10)");
    player.Heal(-10);
    SC_STEP("player.DrawCards(1)");
    player.DrawCards(1);
    cout << "아싸 한장 드로우 근데 아프다" << endl;
    SC_END("Hp_Down_Card_Draw", &player, nullptr);
}

void SupportCard::Card_Draw_Damage_Doun(Player& player, float dmg)
{
    SC_BEGIN("Card_Draw_Damage_Doun", &player, nullptr);
    SC_STEP("player.DrawCards(2)");
    player.DrawCards(2);
    SC_STEP("player.SetNextAtkMultiplier(dmg)");
    player.SetNextAtkMultiplier(dmg);
    cout << "2장 드로우 어? 내팔 왜이러냐?" << endl;
    SC_END("Card_Draw_Damage_Doun", &player, nullptr);
}

void SupportCard::Card_Draw_Disarray(Player& player)
{
    SC_BEGIN("Card_Draw_Disarray", &player, nullptr);
    SC_STEP("player.DrawCards(2)");
    player.DrawCards(2);
    SC_STEP("player.AddDisarrayTurns(1)");
    player.AddDisarrayTurns(1);
    cout << "으아아 머리가 돈다" << endl;

    SC_END("Card_Draw_Disarray", &player, nullptr);

}

void SupportCard::Card_Forsake_Heal_Atk_Change(Player& player, Mob& enemy)
{
    SC_BEGIN("Card_Forsake_Heal_Atk_Change", &player, &enemy);
    if (player.GetHandSize() <= 0)
    {
        SC_SKIP("hand empty");
        SC_END("Card_Forsake_Heal_Atk_Change", &player, &enemy);
        return;
    }
    SC_STEP("player.DiscardHandCardAt(player.ChooseCardIndex())");
    if (!player.DiscardHandCardAt(player.ChooseCardIndex()))
    {
        SC_STEP("fallback: player.DiscardRandomHandCards(1)");
        player.DiscardRandomHandCards(1);
        cout << "이건 뭐냐 무슨 일이 일어나는것이냐!!" << endl;
    }
    SC_STEP("player.AddHealToDamageTurns(1)");
    player.AddHealToDamageTurns(1); // 회복 반전(피해)
    cout << "무슨일이 있었지" << endl;
    SC_END("Card_Forsake_Heal_Atk_Change", &player, &enemy);
}

void SupportCard::TwoCard_Get_Enemy_Card_Get(Player& player, Mob& mob)
{
    SC_BEGIN("TwoCard_Get_Enemy_Card_Get", &player, &mob);
    SC_STEP("player.DrawCards(2)");
    player.DrawCards(2);
    SC_STEP("mob.DrawCards(1)");
    mob.DrawCards(1);
    cout << "일석이조 인가?" << endl;
    SC_END("TwoCard_Get_Enemy_Card_Get", &player, &mob);
}

void SupportCard::Card_Draw_Enemy_Damage_UP(Player& player, Mob& mob)
{
    SC_BEGIN("Card_Draw_Enemy_Damage_UP", &player, &mob);
    SC_STEP("player.DrawCards(2)");
    player.DrawCards(2);
    SC_STEP("player.Damage(2)");
    player.Damage(2);
    cout << "이거만 뽑을게 아자자잣 아프다잉" << endl;
    SC_END("Card_Draw_Enemy_Damage_UP", &player, &mob);
}

void SupportCard::My_Deck_Count_Card_Draw(Player& player)
{
    SC_BEGIN("My_Deck_Count_Card_Draw", &player, nullptr);
    SC_STEP("player.ReturnHandToDeckAndRedraw(true)");
    player.ReturnHandToDeckAndRedraw(true);
    cout << "뭐 어쩌라고?" << endl;
    SC_END("My_Deck_Count_Card_Draw", &player, nullptr);
}

void SupportCard::Sure(Player& player)
{
    SC_BEGIN("Sure", &player, nullptr);
    // 패 1장 버림
    if (player.GetHandSize() > 0)
    {
        cout << "이게 리슨 원이다." << endl;
        SC_STEP("player.DiscardHandCardAt(player.ChooseCardIndex())");
        if (!player.DiscardHandCardAt(player.ChooseCardIndex()))
        {
            SC_STEP("fallback: player.DiscardRandomHandCards(1)");
            player.DiscardRandomHandCards(1);
            cout << "이게 리슨 투다." << endl;
        }
    }
    else
    {
        SC_SKIP("hand empty -> skip discard");
    }

    // 4장 드로우
    SC_STEP("player.DrawCards(4)");
    player.DrawCards(4);

    // 패 3장 덱으로 되돌림
    int sendCount = std::min(3, player.GetHandSize());
    SC_STEP("sendCount=min(3, handSize) -> loop PopRandomHandCard/AddCardToDeck");
    for (int i = 0; i < sendCount; ++i)
    {
        int id = -1;
        if (!player.PopRandomHandCard(id))
        {
            SC_SKIP("PopRandomHandCard failed");
            break;
        }
        player.AddCardToDeck(id, false);
    }

    // 덱 무작위로 보내기
    SC_STEP("player.ShuffleDeck()");
    player.ShuffleDeck();
    SC_END("Sure", &player, nullptr);
}

void SupportCard::Next_AtkCard_Damage_Up(Player& player, float mult)
{
    SC_BEGIN("Next_AtkCard_Damage_Up", &player, nullptr);
    if (mult <= 0.0f) mult = 1.0f;
    SC_STEP("player.SetNextAtkMultiplier(mult)");
    player.SetNextAtkMultiplier(mult);
    cout << "난 강해졌다." << endl;
    SC_END("Next_AtkCard_Damage_Up", &player, nullptr);
}

void SupportCard::MY_Attiravate_Change(Player& player, CAttribute attr, CAttribute neutral)
{
    SC_BEGIN("MY_Attiravate_Change", &player, nullptr);
    SC_STEP("player.SetNextAtkAttribute(attr, neutral)");
    player.SetNextAtkAttribute(attr, neutral);
    cout << "속성 변환" << endl;
    SC_END("MY_Attiravate_Change", &player, nullptr);
}

//void SupportCard::MY_Attiravate_Change_Random(Player& player, CAttribute neutral)
//{
//    player.SetNextAtkAttributeRandom(neutral);
//}

void SupportCard::Card_Forsake_Damage_up(Player& player, float dmg)
{
    SC_BEGIN("Card_Forsake_Damage_up", &player, nullptr);
    // 내 패 2장 버리고
    SC_STEP("player.DiscardRandomHandCards(2)");
    if (!player.DiscardRandomHandCards(2))
    {
        cout << "2장을 버렸느냐?" << endl;
        SC_SKIP("not enough cards to discard 2");
        SC_END("Card_Forsake_Damage_up", &player, nullptr);
        return;
    }
        
    if (dmg <= 0.0f)
    {
        dmg = 2.0f;
        cout << "데미지 2배를 주마" << endl;
    }
    player.SetNextAtkMultiplier(dmg);
    SC_END("Card_Forsake_Damage_up", &player, nullptr);
}

void SupportCard::Atk_Or_Def(Player* player, Mob* mob)
{
    Card* playerLastCard = player->getLastUsedCard();

    if (playerLastCard != nullptr)
    {
        CType lastType = playerLastCard->GetType();
        cout << "신월의 때가 왔다" << endl;
        if (lastType == CType::E_Attack)
        {
            int lastDamage = player->getLastdamageTaken();
            int reflect = lastDamage / 2;
            player->takeDamage(reflect);
            cout << "메이든 인 헤븐" << endl;
        }
        else if (lastType == CType::E_Deffense)
        {
            player->boostNextAttack(20);
            cout << "크아아아아" << endl;
        }
    }
}

void SupportCard::Three_Turn_After_Three_Card(Player& player)
{
    SC_BEGIN("Three_Turn_After_Three_Card", &player, nullptr);
    // 3턴 후 해당 턴에 3장 사용 가능
    SC_STEP("player.SchedulePlayLimit(3, 3)");
    player.SchedulePlayLimit(3, 3);
    cout << "3턴뒤에 온다." << endl;
    SC_END("Three_Turn_After_Three_Card", &player, nullptr);
}

void SupportCard::Used_TwoCard_But_Drow_Prohibition(Player& player)
{
    SC_BEGIN("Used_TwoCard_But_Drow_Prohibition", &player, nullptr);
    // 지금 턴에 2장 더 사용 가능 (기본 1장 + 2장 = 3장)
    SC_STEP("player.AddExtraPlaysThisTurn(2)");
    player.AddExtraPlaysThisTurn(2);

    // 다음 2턴 동안 일반 드로우 불가
    SC_STEP("player.AddNoDrawTurns(2)");
    player.AddNoDrawTurns(2);
    cout << "지금 2장 써 대신 2턴동안 못 뽑아" << endl;
    SC_END("Used_TwoCard_But_Drow_Prohibition", &player, nullptr);
}

void SupportCard::AtkCard_Forsake_Used_TwoCard(Player& player, CardManager& cm)
{
    SC_BEGIN("AtkCard_Forsake_Used_TwoCard", &player, nullptr);
    SC_STEP("cm.DiscardFirstAttackCard()");
    if (!cm.DiscardFirstAttackCard())
    {
        cout << "응 아니야" << endl;
        SC_SKIP("cm.DiscardFirstAttackCard failed (no attack card in hand?)");
        SC_END("AtkCard_Forsake_Used_TwoCard", &player, nullptr);
        return;
    }
        
    SC_STEP("player.AddExtraPlaysThisTurn(2)");
    player.AddExtraPlaysThisTurn(2);
    cout << "응 맞아" << endl;
    SC_END("AtkCard_Forsake_Used_TwoCard", &player, nullptr);
}

void SupportCard::Instant_Turn_Card(Player& player)
{
    SC_BEGIN("Instant_Turn_Card", &player, nullptr);
    SC_STEP("player.TriggerScheduledEffectsNow()");
    player.TriggerScheduledEffectsNow();
    cout << "즉시 발동" << endl;
    SC_END("Instant_Turn_Card", &player, nullptr);
}

void SupportCard::Two_Turn_Heal(Player& player)
{
    SC_BEGIN("Two_Turn_Heal", &player, nullptr);
    SC_STEP("player.AddDelayedHeal(2, heal)");
    player.AddDelayedHeal(2, heal);
    cout << "2턴 뒤 힐" << endl;
    SC_END("Two_Turn_Heal", &player, nullptr);
}

void SupportCard::MY_Two_Card_Forsake_Get_Card(Player& player, Mob& enemy)
{
    SC_BEGIN("MY_Two_Card_Forsake_Get_Card", &player, &enemy);
    // 내 패 2장 버리고
    SC_STEP("player.DiscardRandomHandCards(2)");
    if (!player.DiscardRandomHandCards(2))
    { 
        cout << "2자응 을 버리고" << endl;
        SC_SKIP("not enough cards to discard 2");
        SC_END("MY_Two_Card_Forsake_Get_Card", &player, &enemy);
        return;
    }
        

    // 상대 패에서 무작위 1장 가져오기
    int stolen = -1;
    SC_STEP("enemy.PopRandomHandCard(stolen)");
    if (enemy.PopRandomHandCard(stolen))
    {
        SC_STEP("player.AddCardToHand(stolen)");
        player.AddCardToHand(stolen);
        cout << "너의 1장을 가져간다/" << endl;
    }
    else
    {
        SC_SKIP("enemy hand empty -> PopRandomHandCard failed");
    }

    SC_END("MY_Two_Card_Forsake_Get_Card", &player, &enemy);
        
}

void SupportCard::SeeCard(Player& player)
{
    SC_BEGIN("SeeCard", &player, nullptr);
    // "덱 맨 위 3장을 보고 순서 변경"은 UI 선택이 필요하지만,
    // 여기서는 구현 예시로 "상단 3장 reverse"만 제공합니다.
    std::vector<int> top;
    SC_STEP("player.PeekTopDeck(3, top)");
    if (!player.PeekTopDeck(3, top))
    {
        SC_SKIP("deck has < 3 cards? PeekTopDeck failed");
        SC_END("SeeCard", &player, nullptr);
        return;
    }

    std::reverse(top.begin(), top.end());
    SC_STEP("player.ReorderTopDeck(top)");
    player.ReorderTopDeck(top);
    SC_END("SeeCard", &player, nullptr);
}

void SupportCard::Enemy_Atk_Prohibition(Player& player, Mob& mob)
{
    SC_BEGIN("Enemy_Atk_Prohibition", &player, &mob);
    // mob 으로 변경
    SC_STEP("mob.ProhibitPlay(1)");
    mob.ProhibitPlay(1);
    cout << "봉인" << endl;
    SC_END("Enemy_Atk_Prohibition", &player, &mob);
}

void SupportCard::Enemy_Forsake_Card(Mob& enemy)
{
    SC_BEGIN("Enemy_Forsake_Card", nullptr, &enemy);
    SC_STEP("enemy.DiscardRandomHandCards(1)");
    enemy.DiscardRandomHandCards(1);
    cout << "야당~" << endl;
    SC_END("Enemy_Forsake_Card", nullptr, &enemy);
}

void SupportCard::Deck_Retrun_Card(Player& player, Mob& enemy, int drawEach)
{
    SC_BEGIN("Deck_Retrun_Card", &player, &enemy);
    if (drawEach < 0) drawEach = 0;

    // 양쪽 손패를 덱으로 되돌림
    SC_STEP("player.ReturnHandToDeck(true)");
    player.ReturnHandToDeck(true);
    SC_STEP("enemy.ReturnHandToDeck(true)");
    enemy.ReturnHandToDeck(true);

    // 각자 drawEach 만큼 드로우
    SC_STEP("player.DrawCards(drawEach)");
    player.DrawCards(drawEach);
    SC_STEP("enemy.DrawCards(drawEach)");
    enemy.DrawCards(drawEach);
    SC_END("Deck_Retrun_Card", &player, &enemy);
}

void SupportCard::ApplyByUid(int uid, Player& player, Mob& mob, CardManager& cm)
{
    switch (uid)
    {
    case 200: HealCard(player); break;
    case 201: Heal_Turn_Three_Hp_Down(player); break;
    case 202: RemoveStatus(player); break;
    case 203: GetCard(player, mob); break;
    case 204: Next_AtkCard_Damage_Up(player, 1.5f); break;
    case 205: Three_Turn_After_Three_Card(player); break;
    case 206: Two_Turn_Heal(player); break;
    case 207: Card_Forsake_Card_Draw(player); break;
    case 208: Atk_Or_Def(&player, &mob); break;
    case 209: MY_Two_Card_Forsake_Get_Card(player, mob); break;
    //case 210: MY_Attiravate_Change_Random(player); break; // TODO: 속성별 추가효과
    case 211: Card_Forsake_Damage_up(player, 2.0f); break;
    case 212: Same_Card(player); break;
    case 213: Card_Draw_Enemy_Heal(player, mob); break;
    case 214: Card_Draw_CockroachCard(player); break;
    case 215: Hp_Down_Card_Draw(player); break;
    case 216: SeeCard(player); break;
    case 217: Card_Draw_Damage_Doun(player, 0.5f); break;
    case 218: AtkCard_Forsake_Used_TwoCard(player, cm); break;
    case 219: Card_Draw_Disarray(player); break;
    case 220: Card_Draw_Enemy_Damage_UP(player, mob); break;
    case 221: Sure(player); break;
    case 222: Card_Forsake_Heal_Atk_Change(player, mob); break;
    case 223: Used_TwoCard_But_Drow_Prohibition(player); break;
    case 224: My_Deck_Count_Card_Draw(player); break;
    case 225: Enemy_Atk_Prohibition(player, mob); break; // 공격만 금지로 분리 구현 권장
    case 226: TwoCard_Get_Enemy_Card_Get(player, mob); break;
    case 227: Enemy_Forsake_Card(mob); break;
    case 228: Instant_Turn_Card(player); break;
    case 229: Deck_Retrun_Card(player, mob, /*drawEach=*/3); break; // 원하는 값으로
    default:
        HealCard(player); // 미매핑이면 기본 회복
        break;
    }
}
