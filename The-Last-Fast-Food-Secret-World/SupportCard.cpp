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

SupportCard::SupportCard()
{
    heal = 10;
}

void SupportCard::HealCard(Player& player)
{
    player.Heal(heal);
    cout << "힐을 했습니다." << endl;
}

void SupportCard::Heal_Turn_Three_Hp_Down(Player& player)
{
    
    // 즉시 회복
    player.Heal(heal);

    // 3턴 뒤: (heal + 25%) 만큼 체력 감소
    // (Player::AddDelayedHpLoss 내부에서 1.25 배 적용)
    player.AddDelayedHpLoss(3, heal);
    cout << "3턴뒤 데미지를 입습니다." << endl;
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
    if (!player.HasPoison())
    {
        return;
    }

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
    {
        PoisonDelete(player);
    }
    else
    {
        return;
    }
}

void SupportCard::GetCard(Player& player, Mob& enemy)
{
    int stolenId = -1;
    if (enemy.PopTopDeckCard(stolenId))
    {
        player.AddCardToHand(stolenId);
        cout << "니껀 내꺼 내껀 내꺼 알았냐?" << endl;
        return;
    }

    // 상대 덱도, 버림더미도 비어서 못 가져오는 경우: 아무일도 없음
}

void SupportCard::Card_Forsake_Card_Draw(Player& player)
{
    // "카드 1장을 버린 후 다시 1장 뽑기" (추가 사용횟수 차감 없음)
    // - 선택 카드가 있으면 그걸 버리고, 없으면 무작위 1장 버림
    const int before = player.GetHandSize();
    if (before <= 0)
    {
        return;
    }

    // 선택 카드 버리기 시도
    if (!player.DiscardHandCardAt(player.ChooseCardIndex()))
    {
        player.DiscardRandomHandCards(1);
        cout << "한장 버린다" << endl;
    }
    player.DrawCards(1);
    cout << "1장을 뽑는다." << endl;
}

void SupportCard::Same_Card(Player& player)
{
    // If the most recent two non-support cards (Attack/Defence) that were
    // discarded or played are the same type -> draw 2.
    if (player.HasRecentTwoSameCombatType())
    { 
        player.DrawCards(2);
        cout << "2장 드로우 됐냐?" << endl;
    }
    else
    {
        return;
    }
}


CardManager cm;
Card cd;
void SupportCard::Card_Draw_Enemy_Heal(Player& player, Mob& mob)
{
    // 카드 1장 드로우
    player.DrawCards(1);
    //player.DrawCard(1);
    mob.Heal(10);
    cout << "드로우 그리고 넌 까까 먹어" << endl;
}

void SupportCard::Card_Draw_CockroachCard(Player& player)
{
    player.DrawCards(3);
    player.Heal(20);
    player.AddCardToDeck((int)CardId::Cockroach, true);
    cout << "난 3장 드로우 그리고 바퀴벌레을 꺼내겠다." << endl;
}

void SupportCard::Hp_Down_Card_Draw(Player& player)
{
   
    player.Heal(-10);
    player.DrawCards(1);
    cout << "아싸 한장 드로우 근데 아프다" << endl;
}

void SupportCard::Card_Draw_Damage_Doun(Player& player, float dmg)
{
    player.DrawCards(2);
    player.SetNextAtkMultiplier(dmg);
    cout << "2장 드로우 어? 내팔 왜이러냐?" << endl;
}

void SupportCard::Card_Draw_Disarray(Player& player)
{
    player.DrawCards(2);
    player.AddDisarrayTurns(1);
    cout << "으아아 머리가 돈다" << endl;
}

void SupportCard::Card_Forsake_Heal_Atk_Change(Player& player, Mob& enemy)
{
    if (player.GetHandSize() <= 0)
    {
        return;
    }
   
    if (!player.DiscardHandCardAt(player.ChooseCardIndex()))
    {
     
        player.DiscardRandomHandCards(1);
        cout << "이건 뭐냐 무슨 일이 일어나는것이냐!!" << endl;
    }

    player.AddHealToDamageTurns(1); // 회복 반전(피해)
    cout << "무슨일이 있었지" << endl;
   
}

void SupportCard::TwoCard_Get_Enemy_Card_Get(Player& player, Mob& mob)
{
   
    player.DrawCards(2);
   
    mob.DrawCards(1);
    cout << "일석이조 인가?" << endl;
 
}

void SupportCard::Card_Draw_Enemy_Damage_UP(Player& player, Mob& mob)
{
    // CSV(UID 223): 2장 드로우, 다음 1턴동안 받는 피해 2배
    player.DrawCards(2);
    player.AddDoubleDamageTakenTurns(1);
    cout << "[SUP 223] Draw 2, take double damage for 1 turn" << endl;
}

void SupportCard::My_Deck_Count_Card_Draw(Player& player)
{
   
    player.ReturnHandToDeckAndRedraw(true);
    cout << "뭐 어쩌라고?" << endl;
 
}

void SupportCard::Sure(Player& player)
{
  
    // 패 1장 버림
    if (player.GetHandSize() > 0)
    {
        cout << "이게 리슨 원이다." << endl;
      
        if (!player.DiscardHandCardAt(player.ChooseCardIndex()))
        {
            player.DiscardRandomHandCards(1);
            cout << "이게 리슨 투다." << endl;
        }
    }
    else
    {
        return;
    }

    // 4장 드로우
    player.DrawCards(4);

    // 패 3장 덱으로 되돌림
    int sendCount = std::min(3, player.GetHandSize());
    for (int i = 0; i < sendCount; ++i)
    {
        int id = -1;
        if (!player.PopRandomHandCard(id))
        {
            break;
        }
        player.AddCardToDeck(id, false);
    }

    // 덱 무작위로 보내기
    player.ShuffleDeck();
}

void SupportCard::Next_AtkCard_Damage_Up(Player& player, float mult)
{
   
    if (mult <= 0.0f) mult = 1.0f;
    
    player.SetNextAtkMultiplier(mult);
    cout << "난 강해졌다." << endl;
  
}

void SupportCard::MY_Attiravate_Change(Player& player, CAttribute attr, CAttribute neutral)
{
  
    player.SetNextAtkAttribute(attr, neutral);
    cout << "속성 변환" << endl;
    
}

//void SupportCard::MY_Attiravate_Change_Random(Player& player, CAttribute neutral)
//{
//    player.SetNextAtkAttributeRandom(neutral);
//}

void SupportCard::Card_Forsake_Damage_up(Player& player, float dmg)
{
   
    // 내 패 2장 버리고
 
    if (!player.DiscardRandomHandCards(2))
    {
        cout << "2장을 버렸느냐?" << endl;
     
        return;
    }
        
    if (dmg <= 0.0f)
    {
        dmg = 2.0f;
        cout << "데미지 2배를 주마" << endl;
    }
    player.SetNextAtkMultiplier(dmg);
    
}

void SupportCard::Atk_Or_Def(Player* player, Mob* mob)
{
    // CSV(UID 211): 전 턴에 '상대'가 사용한 카드 타입에 따라 분기
    //  - 상대가 공격을 사용했다면: 내가 직전에 받은 피해의 50%를 상대에게 되돌려줌
    //  - 상대가 방어를 사용했다면: 내 다음 공격 피해 50% 증가(=1.5배)
    Card* oppLast = mob ? mob->getLastUsedCard() : nullptr;
    if (!player || !mob || !oppLast) return;

    CType lastType = oppLast->GetType();

    if (lastType == CType::E_Attack)
    {
        int lastTaken = player->getLastdamageTaken();      // 직전 피격량
        int dmg = lastTaken / 2;
        if (dmg > 0) mob->TakeDamage(dmg);                 // 상대에게 데미지
        cout << "[SUP 211] OppLast=Attack => deal " << dmg << " to opponent" << endl;
    }
    else if (lastType == CType::E_Deffense)
    {
        player->SetNextAtkMultiplier(1.5f);
        cout << "[SUP 211] OppLast=Defense => next atk x1.5" << endl;
    }
}

void SupportCard::Three_Turn_After_Three_Card(Player& player)
{
    
    // 3턴 후 해당 턴에 3장 사용 가능
   
    player.SchedulePlayLimit(3, 3);
    cout << "3턴뒤에 온다." << endl;
    
}

void SupportCard::Used_TwoCard_But_Drow_Prohibition(Player& player)
{
 
    // 지금 턴에 2장 더 사용 가능 (기본 1장 + 2장 = 3장)
  
    player.AddExtraPlaysThisTurn(2);

    // 다음 2턴 동안 일반 드로우 불가
    
    player.AddNoDrawTurns(2);
    cout << "지금 2장 써 대신 2턴동안 못 뽑아" << endl;
  
}

void SupportCard::AtkCard_Forsake_Used_TwoCard(Player& player, CardManager& cm)
{
   
    if (!cm.DiscardFirstAttackCard())
    {
        cout << "응 아니야" << endl;
     
        return;
    }
        
 
    player.AddExtraPlaysThisTurn(2);
    cout << "응 맞아" << endl;

}

void SupportCard::Instant_Turn_Card(Player& player)
{

    player.TriggerScheduledEffectsNow();
    cout << "즉시 발동" << endl;
  
}

void SupportCard::Two_Turn_Heal(Player& player)
{
    
    player.AddDelayedHeal(2, 20);
    cout << "2턴 뒤 힐" << endl;

}

void SupportCard::MY_Two_Card_Forsake_Get_Card(Player& player, Mob& enemy)
{
   
    // 내 패 2장 버리고
   
    if (!player.DiscardRandomHandCards(2))
    { 
        cout << "2자응 을 버리고" << endl;
       
        return;
    }
        

    // 상대 패에서 무작위 1장 가져오기
    int stolen = -1;
    if (enemy.PopRandomHandCard(stolen))
    {
        
        player.AddCardToHand(stolen);
        cout << "너의 1장을 가져간다/" << endl;
    }
    else
    {
        return;
    }

}

void SupportCard::SeeCard(Player& player)
{
    // CSV(UID 219): 패 1장 버림 -> 덱 맨 위 3장 확인 후 순서 변경
    // UI 선택 구현이 없어서 최소 구현으로:
    //  1) 패 1장 랜덤 버림
    //  2) 덱 상단 3장을 "reverse"로 재정렬
    if (!player.DiscardRandomHandCards(1))
        return;

    std::vector<int> top;
    if (!player.PeekTopDeck(3, top))
        return;

    std::reverse(top.begin(), top.end());
    player.ReorderTopDeck(top);
}

void SupportCard::Enemy_Atk_Prohibition(Player& player, Mob& mob)
{
   
    // mob 으로 변경
   
    mob.ProhibitPlay(1);
    cout << "봉인" << endl;
  
}

void SupportCard::Enemy_Forsake_Card(Mob& enemy)
{
   
    enemy.DiscardRandomHandCards(1);
    cout << "야" << endl;
  
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

void SupportCard::ApplyByUid(int uid, Player& player, Mob& mob, CardManager& cm)
{
    switch (uid)
    {
    case 201: HealCard(player); break;
    case 202: HealCard(player); break;
    case 203: HealCard(player); break;
    case 204: Heal_Turn_Three_Hp_Down(player); break;
    case 205: RemoveStatus(player); break;
    case 206: GetCard(player, mob); break;
    case 207: Next_AtkCard_Damage_Up(player, 1.3f); break; // +30%
    case 208: Three_Turn_After_Three_Card(player); break;
    case 209: Two_Turn_Heal(player); break;
    case 210: Card_Forsake_Card_Draw(player); break;
    case 211: Atk_Or_Def(&player, &mob); break;
    case 212: MY_Two_Card_Forsake_Get_Card(player, mob); break;
    //case 213: MY_Attiravate_Change(player); break;
    case 214: Card_Forsake_Damage_up(player, 2.0f); break;
    case 215: Same_Card(player); break;
    case 216: Card_Draw_Enemy_Heal(player, mob); break;
    case 217: Card_Draw_CockroachCard(player); break;
    case 218: Hp_Down_Card_Draw(player); break;
    case 219: SeeCard(player); break;
    case 220: Card_Draw_Damage_Doun(player, 0.5f); break;
    case 221: AtkCard_Forsake_Used_TwoCard(player, cm); break;
    case 222: Card_Draw_Disarray(player); break;
    case 223: Card_Draw_Enemy_Damage_UP(player, mob); break;
    case 224: Sure(player); break;
    case 225: Card_Forsake_Heal_Atk_Change(player, mob); break;
    case 226: Used_TwoCard_But_Drow_Prohibition(player); break;
    case 227: My_Deck_Count_Card_Draw(player); break;
    case 228: Enemy_Atk_Prohibition(player, mob); break;
    case 229: TwoCard_Get_Enemy_Card_Get(player, mob); break;
    case 230: Enemy_Forsake_Card(mob); break;
    case 231: Instant_Turn_Card(player); break;
    case 232: Deck_Retrun_Card(player, mob, /*drawEach=*/3); break;
    case 233:
        cout << "[SUP] uid=233(MAGICLIMIT) sentinel - ignored" << endl;
        break;
    default:
        HealCard(player);
        break;
    }
}
