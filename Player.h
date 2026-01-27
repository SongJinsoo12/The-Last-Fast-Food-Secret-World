#pragma once

#include <vector>
#include <string>
#include <functional>
#include "Card.h"

// WinAPI headers define min/max macros that break std::min/std::max.
// Protect this translation unit and anyone including this header.
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

class Mob;
class AtkCard;
class Player;

// 카드 효과는 (cardId, user, target) 형태로 외부에서 해석/적용
// -> Player는 "카드를 냈다"까지만 처리하고,
//    실제 카드 효과(데미지/힐/특수효과)는 resolver가 cardId로 판단해서 적용하는 구조
using CardResolver_PlayerVsMob = std::function<void(
    int     /*cardId*/,
    Player& /*player*/,
    Mob&    /*mob*/
    )>;

enum class Player_AcquireSource
{
    Normal, // 일반 획득
    SpawnedByRoach // 바퀴벌레 증식으로 생긴 추가분
};

class Player
{
public:
    // 기본 생성자: 테스트/기본값으로 Player를 만들 때 사용
    Player();

    // 이름과 최대 체력을 지정해서 생성할 때 사용
    Player(const std::wstring& name, int maxHp);

    // 현재 체력 조회 (UI표시/전투판정 등에 사용)
    int  GetHP() const;

    // 최대 체력 조회 (HP바 표시, Heal 상한 처리 등에 사용)
    int  GetMaxHP() const;

    // 현재 체력을 강제로 설정 (디버그/리셋/로드 등에 사용)
    // 보통 내부적으로 0~max 범위를 clamp하는 구현을 둡니다.
    void SetHP(int hp);

    // 데미지 적용 (HP 감소). 카드 효과/상태이상 처리 등에서 사용
    void Damage(int dmg);

    // 회복 적용 (HP 증가). 보통 maxHP를 넘지 않도록 제한
    void Heal(int amount);

    // 턴 시작 처리:
    // - 현재 턴 번호 기록
    // - "이번 턴에 쓸 수 있는 카드 수(플레이 리밋)" 초기화
    // - 지연 효과(몇 턴 후 힐/감소) 카운트 감소/발동
    // - 드로우 금지 턴 처리 등
    void BeginTurn(int currentTurn);

    // 턴 종료 처리:
    // - 이번 턴 사용한 카드 수 정리
    // - "이번 턴에 이미 플레이했는지" 플래그 정리 등
    void EndTurn();

    // 생존 여부(HP > 0) 판정
    bool IsAlive() const;

    // 이번 턴에 카드를 더 낼 수 있는지 확인:
    // ex) 기본 1장/턴인데, 3턴 후 3장 사용 가능 같은 효과가 있으면 여기서 허용
    bool CanPlayCard() const;

    // 덱을 카드ID 목록으로 세팅:
    // - 전투 시작 시 덱 구성
    // - shuffle=true면 섞어서 시작
    void SetDeck(const std::vector<int>& cardIds, bool shuffle = true);

    // 덱 셔플(무작위 섞기)
    void ShuffleDeck();

    // 덱에서 count장 뽑아서 손패(hand)에 추가
    // - draw 금지 턴(m_noDrawTurns)일 때는 아무것도 안 뽑도록 구현 가능
    void DrawCards(int count);

    // 손패를 전부 버림 더미(discard)로 이동
    // - 라운드 종료/특정 카드 효과(손패 전부 버리기) 등에 사용
    void DiscardHand();

    // UI에서 클릭/선택한 손패 인덱스를 저장
    void SetSelectedCardIndex(int index);

    // 현재 선택된 카드 인덱스를 반환
    // - UI/입력 시스템에서 "어떤 카드 내는지" 확인용
    int  ChooseCardIndex() const;

    // 실제로 "카드 1장 플레이 시도"를 처리하는 함수
    // - CanPlayCard() 체크
    // - 선택된 카드가 유효한지 체크
    // - 플레이 횟수 차감, 카드 이동(손패->버림) 등 기본 처리
    // - 효과 적용은 resolver(cardId, *this, mob)로 외부에 위임
    //   (카드 로직을 Player 내부에 넣지 않기 위한 구조)
    bool TryPlayOneCard(Mob& mob, const CardResolver_PlayerVsMob& resolver);

    // n턴 후 HP 감소를 예약
    // 예) "체력 ?회복 후 3턴 뒤 ?+25%만큼 체력 감소" 같은 카드에서 사용
    // turnsLater: 몇 턴 후 발동할지
    // healAmount: (변수명은 healAmount지만) 실제로는 감소량/페널티량으로 사용되는 값
    void AddDelayedHpLoss(int turnsLater, int healAmount);

    // n턴 후 힐을 예약
    // 예) "2턴 후 체력 ? 회복" 같은 카드에서 사용
    void AddDelayedHeal(int turnsLater, int amount);

    // n턴 후, 그 턴에 "카드 플레이 제한(playLimit)"을 변경 예약
    // 예) "3턴 후 3장 사용 가능" 같은 카드에서 사용
    void SchedulePlayLimit(int turnsLater, int playLimit);

    // 이번 턴 즉시 추가로 더 플레이 가능한 횟수 부여
    // 예) "이번 턴 추가 1장 더" 같은 효과
    void AddExtraPlaysThisTurn(int extraPlays);

    // 드로우 금지 턴 부여
    // 예) "다음 2턴 동안 드로우 불가" 같은 디버프/패널티 구현
    void AddNoDrawTurns(int turns);

    // 다음 공격 카드 1회에 적용할 배율 저장
    // 예) "자신의 다음 공격 피해량 1.?배 증가" 같은 효과
    void SetNextAtkMultiplier(float mult);

    // 다음 공격 배율이 예약되어 있는지 확인
    bool HasNextAtkMultiplier() const;

    // 실제 공격카드(AtkCard)에 배율을 적용하고,
    // 적용 후 m_hasNextAtkMultiplier를 false로 끄는 식으로 "1회성" 처리하는 용도
    void ApplyNextAttackModifiers(AtkCard& atk);

    // 독 상태인지 확인 (턴 시작 시 DOT 처리, UI 상태 표시 등)
    bool HasPoison() const;

    // 독 상태 on/off
    // 예) "상태 이상 즉시 제거" 카드에서 SetPoison(false)
    void SetPoison(bool on);

    // 손패/덱/버림 더미의 현재 크기 반환 (UI표시, 로직 판단)
    int  GetHandSize() const;
    int  GetDeckSize() const;
    int  GetDiscardSize() const;

    // 손패에 특정 카드ID를 직접 추가
    // 예) 상대 덱에서 카드 훔쳐서 손패로 넣기 / 효과로 생성된 카드 추가
    void AddCardToHand(int cardId);

    // 덱 최상단 카드 1장을 꺼내 outCardId로 반환
    // 성공하면 true, 덱이 비었으면 false
    // -> "상대 덱에서 1장 뽑기" 같은 기능 구현에 핵심
    bool PopTopDeckCard(int& outCardId);

    // 손패에서 무작위 카드 1장을 꺼내 outCardId로 반환
    // 예) "상대 패에서 무작위 카드 1장 가져오기" 같은 효과
    bool PopRandomHandCard(int& outCardId);

    // 손패에서 특정 인덱스 카드를 버림(discard)으로 이동
    // 예) "패 1장 버리고 1장 뽑기" 같은 카드에서 사용
    bool DiscardHandCardAt(int index);

    // 손패에서 무작위로 count장을 버림
    // 예) "패 2장 버린 후 ..." 같은 효과에 사용
    bool DiscardRandomHandCards(int count);

    // 덱 최상단 count장을 "보기"만 하고 out에 담기 (덱은 변화 없음)
    // 예) 덱 서치/미리보기/예지 같은 효과
    bool PeekTopDeck(int count, std::vector<int>& out) const;

    // 덱 최상단 일부를 newOrder 순서로 재배치
    // 예) "덱 위 3장 보고 원하는 순서로 정렬" 같은 효과
    bool ReorderTopDeck(const std::vector<int>& newOrder);

    // 손패를 덱으로 되돌리고(보통 전부), 필요하면 셔플
    // 예) "손패를 덱에 되돌린다" 류 효과, 리셋 효과
    void ReturnHandToDeck(bool shuffleAfter);

    // ====== WinAPI UI 그리기 관련 ======
    // area 범위 안에 Player 관련 UI를 그리는 "상위" 함수(패널+HP+손패 등)
    void Draw(HDC hdc, const RECT& area) const;

    // 플레이어 정보 패널(이름/덱카운트/버림카운트/턴 정보 등)을 그리는 용도
    void DrawPanel(HDC hdc, const RECT& area) const;

    // HP바를 그리는 용도 (현재HP/최대HP 기반)
    void DrawHPBar(HDC hdc, const RECT& barArea) const;

    // 손패 영역에 카드들을 그리는 용도 (m_hand 기반)
    void DrawHand(HDC hdc, const RECT& area) const;

    // ====== "상대가 마지막에 사용한 카드" 추적 ======
    // 상대(혹은 내가) 마지막으로 사용한 카드 포인터 저장
    // 예) "전 턴에 상대가 마지막으로 사용한 카드 종류(공/수)에 따라 효과" 구현에 사용
    void setLastUsedCard(Card* card) { lastUsedCard = card; }

    // 마지막 사용 카드 포인터 반환
    Card* getLastUsedCard() const { return lastUsedCard; }

    // 상대의 마지막 데미지 저장
    void getLastAttackDamage() const { return; }
    void takeDamage(int damage)
    {
        lastDamageTaken = damage;
        m_hp -= damage;
        if (m_hp < 0) m_hp = 0;
        Damage(damage);
    }

    int getLastdamageTaken() const { return lastDamageTaken; }

    // 공격 또는 방어 카드중 상대가 방어를 사용했을때
    void boostNextAttack(int percent)
    {
        nextAttackBoostPercent = percent;
    }

    int getNextAttackBoost() const { return nextAttackBoostPercent; }
    void clearAttackBoost() { nextAttackBoostPercent = 0; }

    // 바퀴벌레 카드 
    void AddCardToDeck(int cardId, bool triggerAcuire = true);

    // 다음 N턴 동안 (턴 종료 시 1씩 감소) Heal이 Damage로 변환
    void AddHealToDamageTurns(int turns);
    bool HasHealToDamageDebuff() const;

private:
    // 플레이어 표시용 이름
    std::wstring m_name = L"Player";

    // 현재/최대 HP
    int m_hp = 1;
    int m_maxHp = 1;

    // 현재 턴 카운터(게임 턴 번호 저장)
    int m_turn = 0;

    // 손패 최대 크기 제한(기본 5장)
    int m_maxHandSize = 5;

    // 카드들은 int 카드ID로 관리
    // m_deck: 덱(뽑을 카드)
    // m_hand: 손패(플레이 가능한 카드)
    // m_discard: 버림 더미
    std::vector<int> m_deck;
    std::vector<int> m_hand;
    std::vector<int> m_discard;

    // 현재 선택된 손패 인덱스(클릭한 카드 등)
    int m_selectedIndex = -1;

    // ====== "턴 당 플레이 가능 카드 수" 관리 ======
    // 기본 1장/턴
    int  m_playLimitThisTurn = 1;

    // 이번 턴에 이미 몇 장 썼는지
    int  m_playsUsedThisTurn = 0;

    // 이번 턴에 플레이했는지 여부(간단 체크용)
    bool m_playedThisTurn = false;

    // ====== "n턴 후 플레이 제한 변경 예약" ======
    bool m_hasScheduledPlayLimit = false;
    int  m_scheduledPlayLimitTurnsLeft = 0;
    int  m_scheduledPlayLimit = 1;

    // ====== "드로우 금지" 효과 ======
    // 0이면 정상 드로우, >0이면 턴마다 감소하면서 드로우 막음
    int m_noDrawTurns = 0;

    // ====== 지연 HP 감소 ======
    // "몇 턴 후 체력 감소" 같은 페널티 처리용
    bool m_hasDelayedHpLoss = false;
    int  m_delayedHpLossTurnsLeft = 0;
    int  m_delayedHpLossAmount = 0;

    // ====== 지연 힐 ======
    bool m_hasDelayedHeal = false;
    int  m_delayedHealTurnsLeft = 0;
    int  m_delayedHealAmount = 0;

    // ====== 상태 이상 ======
    bool m_hasPoison = false;

    // ====== 다음 공격 배율(1회성 버프) ======
    bool  m_hasNextAtkMultiplier = false;
    float m_nextAtkMultiplier = 1.0f;

    // ====== 마지막 사용 카드 추적 ======
    // "전 턴 상대가 공격/수비/보조 중 무엇을 썼는지" 같은 카드 구현에 필요
    Card* lastUsedCard = nullptr;

    // ====== 상대에게 마지막으로 받은 데민지 ======
    int lastDamageTaken = 0;

    // ====== 상대가 방어 사용시 =======
    int nextAttackBoostPercent = 0;

    // 바퀴벌레 카드 관련
    void OnAcquireCard(int cardId);
    void RemoveAllFromHand(int cardId);

    // 힐 -> 데미지
    int m_healToDamageTurnsLeft = 0;
};