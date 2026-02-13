#pragma once

#include <vector>
#include <string>
#include <functional>
#include <unordered_map>
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

// ī ȿ (cardId, user, target) · ܺο ؼ/
// -> Player "ī带 ´" óϰ,
//     ī ȿ(//Ưȿ) resolver cardId Ǵؼ ϴ 
using CardResolver_PlayerVsMob = std::function<void(
    int     /*cardId*/,
    Player& /*player*/,
    Mob&    /*mob*/
    )>;

enum class Player_AcquireSource
{
    Normal, // Ϲ ȹ
    SpawnedByRoach //    ߰
};

struct PlayerDotInfo
{
    int dmg = 0;
    int ticks = 0;
};

class Player
{
public:
    // ⺻ : ׽Ʈ/⺻ Player   
    Player();

    // ̸ ִ ü ؼ   
    Player(const std::wstring& name, int maxHp);

    //  ü ȸ (UIǥ/  )
    int  GetHP() const;

    // ִ ü ȸ (HP ǥ, Heal  ó  )
    int  GetMaxHP() const;

    //  ü   (//ε  )
    //   0~max  clampϴ  Ӵϴ.
    void SetHP(int hp);

    //   (HP ). ī ȿ/̻ ó  
    void Damage(int dmg);

    // ȸ  (HP ).  maxHP  ʵ 
    void Heal(int amount);

    int  GetShield() const;
    void SetShield(int v);
    void AddShield(int v);
    int  TakeDamage(int dmg); // ȯ:  HP  
    void AddDot(int dmg, int ticks);
    void TickDots(); //    ȣ

    // ====== 피해 반사(DEF 143) ======
    void SetReflect(float pct, int hits = 1);
    float GetReflectPct() const;
    int   GetReflectHitsLeft() const;
    void  ConsumeReflectHit();

    //   ó:
    // -   ȣ 
    // - "̹ Ͽ   ִ ī (÷ )" ʱȭ
    // -  ȿ(   /) īƮ /ߵ
    // - ο   ó 
    void BeginTurn(int currentTurn);

    //   ó:
    // - ̹   ī  
    // - "̹ Ͽ ̹ ÷ߴ" ÷  
    void EndTurn();

    //  (HP > 0) 
    bool IsAlive() const;

    // ̹ Ͽ ī带    ִ Ȯ:
    // ex) ⺻ 1/ε, 3  3    ȿ  ⼭ 
    bool CanPlayCard() const;

    //  īID  :
    // -     
    // - shuffle=true  
    void SetDeck(const std::vector<int>& cardIds, bool shuffle = true);

    //  ( )
    void ShuffleDeck();

    //  count ̾Ƽ (hand) ߰
    // - draw  (m_noDrawTurns)  ƹ͵  ̵  
    //void DrawCard(int drawNum); // CardManager ̿ Draw
    void DrawCards(int count); // ü  Draw

    // и   (discard) ̵
    // -  /Ư ī ȿ(  )  
    void DiscardHand();

    // UI Ŭ/  ε 
    void SetSelectedCardIndex(int index);

    //  õ ī ε ȯ
    // - UI/Է ýۿ " ī " Ȯο
    int  ChooseCardIndex() const;

    //  "ī 1 ÷ õ" óϴ Լ
    // - CanPlayCard() üũ
    // - õ ī尡 ȿ üũ
    // - ÷ Ƚ , ī ̵(->)  ⺻ ó
    // - ȿ  resolver(cardId, *this, mob) ܺο 
    //   (ī  Player ο  ʱ  )
    bool TryPlayOneCard(Mob& mob, const CardResolver_PlayerVsMob& resolver);

    // n  HP Ҹ 
    // ) "ü ?ȸ  3  ?+25%ŭ ü "  ī忡 
    // turnsLater:    ߵ
    // healAmount: ( healAmount) δ ҷ/Ƽ Ǵ 
    void AddDelayedHpLoss(int turnsLater, int healAmount);

    // n   
    // ) "2  ü ? ȸ"  ī忡 
    void AddDelayedHeal(int turnsLater, int amount);

    // n ,  Ͽ "ī ÷ (playLimit)"  
    // ) "3  3  "  ī忡 
    void SchedulePlayLimit(int turnsLater, int playLimit);

    // "X턴 뒤 발동" 예약 효과를 즉시 발동 (SUP 231)
    void TriggerDelayedEffectsNow();

    // ̹   ߰  ÷  Ƚ ο
    // ) "̹  ߰ 1 "  ȿ
    void AddExtraPlaysThisTurn(int extraPlays);

    // ο   ο
    // ) " 2  ο Ұ"  /гƼ 
    void AddNoDrawTurns(int turns);

    //   ī 1ȸ   
    // ) "ڽ   ط 1.? "  ȿ
    void SetNextAtkMultiplier(float mult);

    //    Ǿ ִ Ȯ
    bool HasNextAtkMultiplier() const;

    //  ī(AtkCard)  ϰ,
    //   m_hasNextAtkMultiplier false   "1ȸ" óϴ 뵵
    void ApplyNextAttackModifiers(AtkCard& atk);

    //   Ȯ (   DOT ó, UI  ǥ )
    bool HasPoison() const;

    //   on/off
    // ) " ̻  " ī忡 SetPoison(false)
    void SetPoison(bool on);

    // //   ũ ȯ (UIǥ,  Ǵ)
    int  GetHandSize() const;
    int  GetDeckSize() const;
    int  GetDiscardSize() const;

    // п Ư īID  ߰
    // )   ī ļ з ֱ / ȿ  ī ߰
    void AddCardToHand(int cardId);

    //  ֻ ī 1  outCardId ȯ
    // ϸ true,   false
    // -> "  1 ̱"    ٽ
    bool PopTopDeckCard(int& outCardId);

    // п  ī 1  outCardId ȯ
    // ) " п  ī 1 "  ȿ
    bool PopRandomHandCard(int& outCardId);

    // п Ư ε ī带 (discard) ̵
    // ) " 1  1 ̱"  ī忡 
    bool DiscardHandCardAt(int index);

    // п  count 
    // ) " 2   ..."  ȿ 
    bool DiscardRandomHandCards(int count);

    //  ֻ count "" ϰ out  ( ȭ )
    // )  ġ/̸/  ȿ
    bool PeekTopDeck(int count, std::vector<int>& out) const;

    //  ֻ Ϻθ newOrder  ġ
    // ) "  3  ϴ  "  ȿ
    bool ReorderTopDeck(const std::vector<int>& newOrder);

    // и  ǵ( ), ʿϸ 
    // ) "и  ǵ"  ȿ,  ȿ
    void ReturnHandToDeck(bool shuffleAfter);

    // ====== WinAPI UI ׸  ======
    // area  ȿ Player  UI ׸ "" Լ(г+HP+ )
    void Draw(HDC hdc, const RECT& area) const;

    // ÷̾  г(̸/īƮ/īƮ/  ) ׸ 뵵
    void DrawPanel(HDC hdc, const RECT& area) const;

    // HPٸ ׸ 뵵 (HP/ִHP )
    void DrawHPBar(HDC hdc, const RECT& barArea) const;

    //   ī ׸ 뵵 (m_hand )
    void DrawHand(HDC hdc, const RECT& area) const;

    // ====== "밡   ī"  ======
    // (Ȥ )   ī  
    // ) " Ͽ 밡   ī (/)  ȿ"  
    void setLastUsedCard(Card* card) { lastUsedCard = card; }

    //   ī  ȯ
    Card* getLastUsedCard() const { return lastUsedCard; }



    // Recent combat-type history (Attack/Defence only). Support (E_Magic) is ignored.
    // You must register cardId -> CType somewhere at init time so the engine can classify cards.
    // Example:
    //   Player::RegisterCardType(101, CType::E_Attack);
    //   Player::RegisterCardType(201, CType::E_Deffence);
    static void RegisterCardType(int cardId, CType type);

    // Returns true if the most recent two non-support (Attack/Defence) cards that were
    // discarded OR played are the same type.
    bool HasRecentTwoSameCombatType() const;

    //    
    void getLastAttackDamage() const { return; }
    void takeDamage(int damage)
    {
        TakeDamage(damage);
    }

    int getLastdamageTaken() const { return lastDamageTaken; }

    //  Ǵ  ī 밡  
    void boostNextAttack(int percent)
    {
        nextAttackBoostPercent = percent;
    }

    int getNextAttackBoost() const { return nextAttackBoostPercent; }
    void clearAttackBoost() { nextAttackBoostPercent = 0; }

    //  ī 
    void AddCardToDeck(int cardId, bool triggerAcuire = true);

    //  N  (   1 ) Heal Damage ȯ
    void AddHealToDamageTurns(int turns);
    bool HasHealToDamageDebuff() const;

    // ̻ ȥ
        // 다음 N턴 동안 받는 피해 2배
    void AddDoubleDamageTakenTurns(int turns);
    bool HasDoubleDamageTakenDebuff() const;

void AddDisarrayTurns(int turns);
    bool IsDisarray() const;
    int GetDisarrayTurnLeft() const;

    // и  ְ,  ŭ ٽ ο
    void ReturnHandToDeckAndRedraw(bool shuffleAfter = true);

    //   ߵ
    void TriggerScheduledEffectsNow();

    // Ӽ ȯ
    void SetNextAtkAttribute(CAttribute attr, CAttribute neutral = E_BREAD);
    void SetNextAtkAttributeRandom(CAttribute neutral = E_BREAD);
    bool HasNextAtkAttributeOverride() const;

    //  
    void ProhibitPlay(int turns);
    void ProhibitAttackPlay(int turns);
    bool IsPlayProhibited() const;
    bool IsAttackPlayProhibited() const;

    // ڵ Ȯο
    int GetPlayLimitThisTurn() const { return m_playLimitThisTurn; }



    // --- Bridge accessors for CardManager syncing (int cardId deck/hand) ---
    const std::vector<int>& Debug_GetDeckIds() const { return m_deck; }
    const std::vector<int>& Debug_GetHandIds() const { return m_hand; }
    void Debug_SetDeckIds(const std::vector<int>& ids) { m_deck = ids; }
    void Debug_SetHandIds(const std::vector<int>& ids) { m_hand = ids; }

private:
    // CardManager public 
    // ÷̾ ǥÿ ̸
    std::wstring m_name = L"Player";

    // /ִ HP
    int m_hp = 50;
    int m_maxHp = 50;

    // ǵ
    int m_shield = 0;

    // 피해 반사 버프(DEF 143): 다음 피격 hits회 동안 받은 피해의 pct만큼 반사
    float m_reflectPct = 0.0f;
    int   m_reflectHitsLeft = 0;

    // Ʈ
    std::vector<PlayerDotInfo> p_dots;

    //   ī(  ȣ )
    int m_turn = 0;

    //  ִ ũ (⺻ 5)
    int m_maxHandSize = 5;

    // ī int īID 
    // m_deck: ( ī)
    // m_hand: (÷  ī)
    // m_discard:  
    std::vector<int> m_deck;
    std::vector<int> m_hand;
    std::vector<int> m_discard;

    //  õ  ε(Ŭ ī )
    int m_selectedIndex = -1;

    // ====== "  ÷  ī "  ======
    // ⺻ 1/
    int  m_playLimitThisTurn = 1;

    // ̹ Ͽ ̹   
    int  m_playsUsedThisTurn = 0;

    // ̹ Ͽ ÷ߴ ( üũ)
    bool m_playedThisTurn = false;

    // ====== "n  ÷   " ======
    bool m_hasScheduledPlayLimit = false;
    int  m_scheduledPlayLimitTurnsLeft = 0;
    int  m_scheduledPlayLimit = 1;

    // ====== "ο " ȿ ======
    // 0̸  ο, >0̸ ϸ ϸ鼭 ο 
    int m_noDrawTurns = 0;

    // ======  HP  ======
    // "   ü "  Ƽ ó
    bool m_hasDelayedHpLoss = false;
    int  m_delayedHpLossTurnsLeft = 0;
    int  m_delayedHpLossAmount = 0;

    // ======   ======
    bool m_hasDelayedHeal = false;
    int  m_delayedHealTurnsLeft = 0;
    int  m_delayedHealAmount = 0;

    // ======  ̻ ======
    bool m_hasPoison = false;

    // ======   (1ȸ ) ======
    bool  m_hasNextAtkMultiplier = false;
    float m_nextAtkMultiplier = 1.0f;

    // ======   ī  ======
    // "  밡 //   "  ī  ʿ
    Card* lastUsedCard = nullptr;

    // ====== 뿡    ======
    int lastDamageTaken = 0;
    int m_doubleDamageTurns = 0; // 받는 피해 2배 남은 턴


    // ====== 밡   =======
    int nextAttackBoostPercent = 0;

    //  ī 
    void OnAcquireCard(int cardId);
    void RemoveAllFromHand(int cardId);

    //  -> 
    int m_healToDamageTurnsLeft = 0;

    // ̻ ȥ
    int m_disarrayTurnsLeft = 0;

    // Ӽȯ
    bool m_hasNextAtkAttributeOverride = false;
    CAttribute m_nextAtkAttributeOverride = E_BREAD;

    // --- Recent combat-type history ---
    // Stores the last two non-support card types that were moved to discard (by playing or discarding).
    // m_recentCombatCount is 0..2.
    CType m_recentCombatTypes[2] = { CType::E_Magic, CType::E_Magic };
    int   m_recentCombatCount = 0;

    // Internal: record one cardId into recent history (Attack/Defence only).
    void RecordCardHistory(int cardId);

    // Internal: resolve cardId -> CType (returns E_Magic if unknown).
    static CType ResolveCardTypeById(int cardId);

    //  ī 
    int m_playProhibitTurnsLeft = 0;
    int m_attackProhibitTurnsLeft = 0;
};