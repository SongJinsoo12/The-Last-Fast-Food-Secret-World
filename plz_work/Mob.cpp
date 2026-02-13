#include "Mob.h"
#include "Player.h"
#include "AtkCard.h"
#include "Card.h"

#include <algorithm>
#include <random>
#include <sstream>
#include <unordered_map>

static int ClampInt(int v, int lo, int hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

// ===== CardId -> CType registry (needed for history checks) =====
static std::unordered_map<int, CType>& CardTypeTable()
{
    static std::unordered_map<int, CType> table;
    // Minimal default: Cockroach behaves like support (ignored in history).
    table[(int)CardId::Cockroach] = CType::E_Magic;
    return table;
}

void Mob::RegisterCardType(int cardId, CType type)
{
    CardTypeTable()[cardId] = type;
}

CType Mob::ResolveCardTypeById(int cardId)
{
    auto& t = CardTypeTable();
    auto it = t.find(cardId);
    if (it != t.end())
        return it->second;
    return CType::E_Magic; // unknown -> treat as support (ignored)
}

void Mob::RecordCardHistory(int cardId)
{
    const CType t = ResolveCardTypeById(cardId);
    if (t == CType::E_Magic)
        return; // support ignored

    if (m_recentCombatCount < 2)
    {
        m_recentCombatTypes[m_recentCombatCount++] = t;
    }
    else
    {
        m_recentCombatTypes[0] = m_recentCombatTypes[1];
        m_recentCombatTypes[1] = t;
    }
}

bool Mob::HasRecentTwoSameCombatType() const
{
    if (m_recentCombatCount < 2)
        return false;
    return m_recentCombatTypes[0] == m_recentCombatTypes[1];
}

Mob::Mob() {}

Mob::Mob(const std::wstring& name, int maxHp)
{
    m_name = name;
    m_maxHp = (maxHp <= 1) ? 1 : maxHp;
    m_hp = m_maxHp;
}

int Mob::GetHP() const { return m_hp; }

int Mob::GetMaxHP() const
{
    return m_maxHp;
}

void Mob::SetHP(int hp)
{
    m_hp = ClampInt(hp, 0, m_maxHp);
}

void Mob::Damage(int dmg)
{
    if (dmg < 0) dmg = 0;
    lastDamageTaken = dmg;
    SetHP(m_hp - dmg);
    RemoveAllFromHand((int)CardId::Cockroach);
}

void Mob::Heal(int amount)
{
    if (amount < 0) amount = 0;

    if (amount < 0)
    {
        Damage(-amount);
        return;
    }

    if (m_healToDamageTurnsLeft)
    {
        Damage(amount);
        return;
    }
    SetHP(m_hp + amount);
}

int Mob::GetShield() const
{
    return m_shield;
}

void Mob::SetShield(int v)
{
    if (v < 0) v = 0;
    m_shield = v;
}

void Mob::AddShield(int v)
{
    if (v <= 0) return;
    m_shield += v;
}


void Mob::SetReflect(float pct, int hits)
{
    if (pct <= 0.0f || hits <= 0)
    {
        m_reflectPct = 0.0f;
        m_reflectHitsLeft = 0;
        return;
    }
    m_reflectPct = pct;
    m_reflectHitsLeft = hits;
}

float Mob::GetReflectPct() const
{
    return m_reflectPct;
}

int Mob::GetReflectHitsLeft() const
{
    return m_reflectHitsLeft;
}

void Mob::TriggerDelayedEffectsNow()
{
    // 즉시 처리: "n턴 뒤 발동"으로 예약된 것들을 지금 발동
    if (m_hasDelayedHpLoss)
    {
        Damage(m_delayedHpLossAmount);
        m_hasDelayedHpLoss = false;
        m_delayedHpLossTurnsLeft = 0;
    }
    if (m_hasDelayedHeal)
    {
        Heal(m_delayedHealAmount);
        m_hasDelayedHeal = false;
        m_delayedHealTurnsLeft = 0;
    }
    if (m_hasScheduledPlayLimit)
    {
        m_playLimitThisTurn = std::max(1, m_scheduledPlayLimit);
        m_hasScheduledPlayLimit = false;
        m_scheduledPlayLimitTurnsLeft = 0;
    }
}
int Mob::TakeDamage(int dmg)
{
    if (dmg <= 0) return 0;

    // 받는 피해 2배 디버프
    if (m_doubleDamageTurns > 0) dmg *= 2;

    int absorbed = 0;
    if (m_shield > 0)
    {
        absorbed = (m_shield >= dmg) ? dmg : m_shield;
        m_shield -= absorbed;
        dmg -= absorbed;
    }

    if (dmg <= 0) return 0;

    // 받는 피해 2배 디버프
    if (m_doubleDamageTurns > 0) dmg *= 2;

    // HP Ҵ  Damage()  (Roach   ΰȿ )
    int before = m_hp;
    Damage(dmg);
    lastDamageTaken = before - m_hp;
    return lastDamageTaken;
}

void Mob::AddDot(int dmg, int ticks)
{
    if (dmg <= 0 || ticks <= 0) return;
    m_dots.push_back({ dmg, ticks });
}

void Mob::TickDots()
{
    if (m_dots.empty()) return;

    int total = 0;

    for (size_t i = 0; i < m_dots.size();)
    {
        MobDotInfo& d = m_dots[i];
        if (d.ticks > 0)
        {
            total += d.dmg;
            d.ticks--;
        }

        if (d.ticks <= 0)
            m_dots.erase(m_dots.begin() + i);
        else
            ++i;
    }

    if (total > 0)
    {
        // DOT "ǵ " ϰ  Damage(total) ȣ
        // DOT ǵ忡  ϰ  TakeDamage(total) ȣ
        Damage(total);
    }
}

bool Mob::IsAlive() const { return m_hp > 0; }

void Mob::BeginTurn(int currentTurn)
{
    m_turn = currentTurn;
    SetShield(0);
    SetReflect(0.0f, 0); // 반사 버프 초기화

    // ---- ÷ Ƚ ʱȭ ----
    m_playsUsedThisTurn = 0;
    m_playLimitThisTurn = 1;

    //  "n  ÷  " ó
    if (m_hasScheduledPlayLimit)
    {
        --m_scheduledPlayLimitTurnsLeft;
        if (m_scheduledPlayLimitTurnsLeft <= 0)
        {
            m_playLimitThisTurn = std::max(1, m_scheduledPlayLimit);
            m_hasScheduledPlayLimit = false;
        }
    }

    m_playedThisTurn = (m_playsUsedThisTurn >= m_playLimitThisTurn);

    //     ʱȭ()
    m_selectedIndex = -1;

    // ---- ο ó (ο  ̸ ŵ) ----
    if (m_noDrawTurns > 0)
    {
        --m_noDrawTurns;
    }
    else
    {
        if ((int)m_hand.size() < m_maxHandSize)
            DrawCards(1);
    }

    // ---- 받는 피해 2배 디버프 ----
    if (m_doubleDamageTurns > 0)
        --m_doubleDamageTurns;

    // ----  HP  ó ----
    if (m_hasDelayedHpLoss)
    {
        --m_delayedHpLossTurnsLeft;
        if (m_delayedHpLossTurnsLeft <= 0)
        {
            Damage(m_delayedHpLossAmount);
            m_hasDelayedHpLoss = false;
        }
    }

    // ----   ó ----
    if (m_hasDelayedHeal)
    {
        --m_delayedHealTurnsLeft;
        if (m_delayedHealTurnsLeft <= 0)
        {
            Heal(m_delayedHealAmount);
            m_hasDelayedHeal = false;
        }
    }
}

void Mob::EndTurn()
{
    if (m_healToDamageTurnsLeft > 0)
    {
        --m_healToDamageTurnsLeft;
    }

    if (m_disarrayTurnsLeft > 0)
    {
        --m_disarrayTurnsLeft;
    }
    if (m_playProhibitTurnsLeft > 0)
    {
        --m_playProhibitTurnsLeft;
    }
    if (m_attackProhibitTurnsLeft > 0)
    {
        --m_attackProhibitTurnsLeft;
    }
    // ٸ   ó
}

bool Mob::CanPlayCard() const
{
    if (!IsAlive()) return false;
    if (m_hand.empty()) return false;

    // ÷ 
    if (m_playsUsedThisTurn >= m_playLimitThisTurn) return false;

    // ÷̾  
    if (m_selectedIndex < 0 || m_selectedIndex >= (int)m_hand.size()) return false;
    return true;
}

void Mob::SetDeck(const std::vector<int>& cardIds, bool shuffle)
{
    m_deck = cardIds;
    m_hand.clear();
    m_discard.clear();
    m_selectedIndex = -1;
    if (shuffle) ShuffleDeck();
}

void Mob::ShuffleDeck()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::shuffle(m_deck.begin(), m_deck.end(), gen);
}

//CardManager cm;
//void Player::DrawCard(int drawNum)
//{
//    cm.CardDraw(drawNum);
//}

void Mob::DrawCards(int count)
{
    for (int i = 0; i < count; ++i)
    {
        if ((int)m_hand.size() >= m_maxHandSize)
            return;

        if (m_deck.empty())
        {
            //   ̸ 
            if (m_discard.empty())
                return;

            m_deck = m_discard;
            m_discard.clear();
            ShuffleDeck();
        }

        int top = m_deck.back();
        m_deck.pop_back();
        m_hand.push_back(top);
    }
}

void Mob::DiscardHand()
{
    for (int id : m_hand)
    {
        m_discard.push_back(id);
        RecordCardHistory(id);
    }
    m_hand.clear();
    m_selectedIndex = -1;
}

void Mob::SetSelectedCardIndex(int index)
{
    if (index < 0 || index >= (int)m_hand.size())
    {
        m_selectedIndex = -1;
        return;
    }
    m_selectedIndex = index;
}

int Mob::ChooseCardIndex() const
{
    if (m_selectedIndex < 0 || m_selectedIndex >= (int)m_hand.size())
        return -1;
    return m_selectedIndex;
}

bool Mob::TryPlayOneCard(Player& player, const CardResolver_MobVsPlayer& resolver)
{
    // Basic checks
    if (!IsAlive()) return false;
    if (m_hand.empty()) return false;
    if (m_playsUsedThisTurn >= m_playLimitThisTurn) return false;

    int idx = -1;

    // Disarray: pick random card
    if (IsDisarray())
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, (int)m_hand.size() - 1);
        idx = dist(gen);
    }
    else
    {
        idx = ChooseCardIndex();
        if (idx < 0 || idx >= (int)m_hand.size())
            return false;
    }

    const int cardId = m_hand[idx];

    // Cockroach: cannot be played (keep in hand)
    if (cardId == (int)CardId::Cockroach)
    {
        m_selectedIndex = -1;
        return false;
    }

    // Move hand -> discard
    m_hand.erase(m_hand.begin() + idx);
    m_discard.push_back(cardId);
    RecordCardHistory(cardId);

    // Reset selection
    m_selectedIndex = -1;

    // Resolve effect externally
    if (resolver)
        resolver(cardId, *this, player);

    // Count play
    ++m_playsUsedThisTurn;
    m_playedThisTurn = (m_playsUsedThisTurn >= m_playLimitThisTurn);
    return true;
}

void Mob::AddDelayedHpLoss(int turnsLater, int healAmount)
{
    m_hasDelayedHpLoss = true;
    m_delayedHpLossTurnsLeft = turnsLater;

    // (heal + 25%) 
    m_delayedHpLossAmount = static_cast<int>(healAmount * 1.25f);
}

void Mob::AddDelayedHeal(int turnsLater, int amount)
{
    m_hasDelayedHeal = true;
    m_delayedHealTurnsLeft = turnsLater;
    m_delayedHealAmount = amount;
}

void Mob::SchedulePlayLimit(int turnsLater, int playLimit)
{
    m_hasScheduledPlayLimit = true;
    m_scheduledPlayLimitTurnsLeft = turnsLater;
    m_scheduledPlayLimit = playLimit;
}

void Mob::AddExtraPlaysThisTurn(int extraPlays)
{
    if (extraPlays <= 0) return;
    m_playLimitThisTurn += extraPlays;
    if (m_playLimitThisTurn < 1) m_playLimitThisTurn = 1;
}

void Mob::AddNoDrawTurns(int turns)
{
    if (turns <= 0) return;
    //  (   켱)
    m_noDrawTurns = std::max(m_noDrawTurns, turns);
}

void Mob::SetNextAtkMultiplier(float mult)
{
    if (mult <= 0.0f) return;
    m_hasNextAtkMultiplier = true;
    m_nextAtkMultiplier = mult;
}

bool Mob::HasNextAtkMultiplier() const
{
    return m_hasNextAtkMultiplier;
}

void Mob::ApplyNextAttackModifiers(AtkCard& atk)
{
    if (!m_hasNextAtkMultiplier) return;

    atk.Damage = static_cast<int>(atk.Damage * m_nextAtkMultiplier);
    m_hasNextAtkMultiplier = false;
    m_nextAtkMultiplier = 1.0f;

    if (m_hasNextAtkAttributeOverride)
    {
        atk.setAttribute(m_nextAtkAttributeOverride);
        m_hasNextAtkAttributeOverride = false;
        m_nextAtkAttributeOverride = E_BREAD;
    }
}

bool Mob::HasPoison() const { return m_hasPoison; }
void Mob::SetPoison(bool on) { m_hasPoison = on; }

int Mob::GetHandSize() const { return (int)m_hand.size(); }
int Mob::GetDeckSize() const { return (int)m_deck.size(); }
int Mob::GetDiscardSize() const { return (int)m_discard.size(); }

void Mob::AddCardToHand(int cardId)
{
    if ((int)m_hand.size() >= m_maxHandSize)
    {
        // а ̸  ̷ 
        m_discard.push_back(cardId);

        //   ȹ ȹǷ Ʈ 
        OnAcquireCard(cardId);
        return;
    }
    m_hand.push_back(cardId);

    // տ  ͵ ȹ̹Ƿ Ʈ
    OnAcquireCard(cardId);
}

bool Mob::PopTopDeckCard(int& outCardId)
{
    if (m_deck.empty())
    {
        if (m_discard.empty()) return false;
        m_deck = m_discard;
        m_discard.clear();
        ShuffleDeck();
    }

    outCardId = m_deck.back();
    m_deck.pop_back();
    return true;
}

bool Mob::PopRandomHandCard(int& outCardId)
{
    if (m_hand.empty()) return false;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, (int)m_hand.size() - 1);

    int idx = dist(gen);
    outCardId = m_hand[idx];
    m_hand.erase(m_hand.begin() + idx);
    return true;
}

bool Mob::DiscardHandCardAt(int index)
{
    if (index < 0 || index >= (int)m_hand.size()) return false;

    const int id = m_hand[index];
    m_hand.erase(m_hand.begin() + index);
    m_discard.push_back(id);

    if (m_selectedIndex == index) m_selectedIndex = -1;
    if (m_selectedIndex > index)  --m_selectedIndex;

    return true;
}

bool Mob::DiscardRandomHandCards(int count)
{
    if (count <= 0) return true;
    if ((int)m_hand.size() < count) return false;

    for (int i = 0; i < count; ++i)
    {
        int dummy = -1;
        if (!PopRandomHandCard(dummy))
            return false;
        m_discard.push_back(dummy);
        RecordCardHistory(dummy);
    }
    return true;
}

bool Mob::PeekTopDeck(int count, std::vector<int>& out) const
{
    out.clear();
    if (count <= 0) return false;
    if ((int)m_deck.size() < count) return false;

    out.reserve(count);
    for (int i = 0; i < count; ++i)
    {
        out.push_back(m_deck[m_deck.size() - 1 - i]);
    }
    // out[0] = top
    return true;
}

bool Mob::ReorderTopDeck(const std::vector<int>& newOrder)
{
    const int n = (int)newOrder.size();
    if (n <= 0) return false;
    if ((int)m_deck.size() < n) return false;

    //  top n 
    for (int i = 0; i < n; ++i)
        m_deck.pop_back();

    // newOrder[0] top ǵ  push
    for (int i = n - 1; i >= 0; --i)
        m_deck.push_back(newOrder[i]);

    return true;
}

void Mob::ReturnHandToDeck(bool shuffleAfter)
{
    for (int id : m_hand)
        m_deck.push_back(id);
    m_hand.clear();
    m_selectedIndex = -1;

    if (shuffleAfter) ShuffleDeck();
}

// ----------------- WinAPI Drawing -----------------

void Mob::Draw(HDC hdc, const RECT& area) const
{
    DrawPanel(hdc, area);

    //  ǥ (г Ʒ)
    RECT handArea = area;
    handArea.top = area.bottom - 40;
    DrawHand(hdc, handArea);
}

void Mob::DrawPanel(HDC hdc, const RECT& area) const
{
    Rectangle(hdc, area.left, area.top, area.right, area.bottom);
    SetBkMode(hdc, TRANSPARENT);

    // ̸
    RECT nameRc = area;
    nameRc.left += 8;
    nameRc.top += 6;
    nameRc.bottom = nameRc.top + 20;
    DrawTextW(hdc, m_name.c_str(), -1, &nameRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // HP ؽƮ
    std::wstringstream ss;
    ss << L"HP: " << m_hp << L"/" << m_maxHp;

    RECT hpTextRc = area;
    hpTextRc.left += 8;
    hpTextRc.top += 28;
    hpTextRc.bottom = hpTextRc.top + 20;
    const std::wstring hpText = ss.str();
    DrawTextW(hdc, hpText.c_str(), -1, &hpTextRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    //  ؽƮ
    std::wstringstream ts;
    ts << L"Turn: " << m_turn << L" (" << m_playsUsedThisTurn << L"/" << m_playLimitThisTurn << L")";
    RECT turnRc = area;
    turnRc.left += 8;
    turnRc.top += 48;
    turnRc.bottom = turnRc.top + 20;
    const std::wstring turnText = ts.str();
    DrawTextW(hdc, turnText.c_str(), -1, &turnRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // HP 
    RECT barRc = area;
    barRc.left += 8;
    barRc.right -= 8;
    barRc.top = area.top + 72;
    barRc.bottom = barRc.top + 14;
    DrawHPBar(hdc, barRc);
}

void Mob::DrawHPBar(HDC hdc, const RECT& barArea) const
{
    Rectangle(hdc, barArea.left, barArea.top, barArea.right, barArea.bottom);

    const int w = (barArea.right - barArea.left) - 2;
    const int h = (barArea.bottom - barArea.top) - 2;

    float ratio = 0.0f;
    if (m_maxHp > 0) ratio = (float)m_hp / (float)m_maxHp;
    if (ratio < 0.0f) ratio = 0.0f;
    if (ratio > 1.0f) ratio = 1.0f;

    RECT fill = barArea;
    fill.left += 1;
    fill.top += 1;
    fill.bottom = fill.top + h;
    fill.right = fill.left + (int)(w * ratio);

    HBRUSH br = CreateSolidBrush(RGB(80, 120, 220));
    FillRect(hdc, &fill, br);
    DeleteObject(br);
}

void Mob::DrawHand(HDC hdc, const RECT& area) const
{
    Rectangle(hdc, area.left, area.top, area.right, area.bottom);
    SetBkMode(hdc, TRANSPARENT);

    RECT textRc = area;
    textRc.left += 6;
    textRc.top += 6;

    std::wstringstream ss;
    ss << L"Hand: ";

    for (size_t i = 0; i < m_hand.size(); ++i)
    {
        if ((int)i == m_selectedIndex) ss << L"[";
        ss << m_hand[i];
        if ((int)i == m_selectedIndex) ss << L"]";
        if (i + 1 < m_hand.size()) ss << L", ";
    }

    const std::wstring s = ss.str();
    DrawTextW(hdc, s.c_str(), -1, &textRc, DT_LEFT | DT_TOP | DT_SINGLELINE);
}
// ---------------------------------------------------------------------------
void Mob::AddCardToDeck(int cardId, bool triggerAcuire)
{
    m_deck.push_back(cardId);
    if (triggerAcuire)
    {
        OnAcquireCard(cardId);
    }
}

void Mob::AddHealToDamageTurns(int turns)
{
    if (turns <= 0)
    {
        return;
    }
    m_healToDamageTurnsLeft = std::max(m_healToDamageTurnsLeft, turns);

}

bool Mob::HasHealToDamageDebuff() const
{
    return m_healToDamageTurnsLeft > 0;
}

void Mob::AddDoubleDamageTakenTurns(int turns)
{
    if (turns <= 0) return;
    m_doubleDamageTurns = std::max(m_doubleDamageTurns, turns);
}
bool Mob::HasDoubleDamageTakenDebuff() const
{
    return m_doubleDamageTurns > 0;
}

void Mob::AddDisarrayTurns(int turns)
{
    if (turns <= 0)
    {
        return;
    }
    m_disarrayTurnsLeft = std::max(m_disarrayTurnsLeft, turns);
}

bool Mob::IsDisarray() const
{
    return m_disarrayTurnsLeft > 0;
}

int Mob::GetDisarrayTurnLeft() const
{
    return m_disarrayTurnsLeft;
}

void Mob::ReturnHandToDeckAndRedraw(bool shuffleAfter)
{
    const int n = GetHandSize();
    ReturnHandToDeck(shuffleAfter);
    DrawCards(n);
}

void Mob::TriggerScheduledEffectsNow()
{
    if (m_hasDelayedHpLoss)
    {
        Damage(m_delayedHpLossAmount);
        m_hasDelayedHpLoss = false;
        m_delayedHpLossTurnsLeft = 0;
        m_delayedHpLossAmount = 0;
    }

    if (m_hasDelayedHeal)
    {
        Heal(m_delayedHealAmount);
        m_hasDelayedHeal = false;
        m_delayedHealTurnsLeft = 0;
        m_delayedHealAmount = 0;
    }

    if (m_hasScheduledPlayLimit)
    {
        m_playLimitThisTurn = std::max(1, m_scheduledPlayLimit);
        m_hasScheduledPlayLimit = false;
        m_scheduledPlayLimitTurnsLeft = 0;
        m_scheduledPlayLimit = 1;
    }
}

void Mob::SetNextAtkAttribute(CAttribute attr, CAttribute neutral)
{
    if (attr == neutral) return;
    m_hasNextAtkAttributeOverride = true;
    m_nextAtkAttributeOverride = attr;
}

void Mob::SetNextAtkAttributeRandom(CAttribute neutral)
{
    std::vector<CAttribute> pool = { E_BULGOGI, E_SOURCE, E_CHESSE, E_VEGAT, E_BREAD };
    pool.erase(std::remove(pool.begin(), pool.end(), neutral), pool.end());
    if (pool.empty()) return;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, (int)pool.size() - 1);

    SetNextAtkAttribute(pool[dist(gen)], neutral);
}

bool Mob::HasNextAtkAttributeOverride() const
{
    return m_hasNextAtkAttributeOverride;
}

void Mob::ProhibitPlay(int turns)
{
    if (turns <= 0) return;
    m_playProhibitTurnsLeft = std::max(m_playProhibitTurnsLeft, turns);
}

void Mob::ProhibitAttackPlay(int turns)
{
    if (turns <= 0) return;
    m_attackProhibitTurnsLeft = std::max(m_attackProhibitTurnsLeft, turns);
}

bool Mob::IsPlayProhibited() const
{
    return m_playProhibitTurnsLeft > 0;
}

bool Mob::IsAttackPlayProhibited() const
{
    return m_attackProhibitTurnsLeft > 0;
}

void Mob::OnAcquireCard(int cardId)
{
    if (cardId == (int)CardId::Cockroach)
    {
        //  ȹϸ  ߰
        //   
        AddCardToDeck((int)CardId::Cockroach, false);
    }
}

void Mob::RemoveAllFromHand(int cardId)
{
    m_hand.erase(std::remove(m_hand.begin(), m_hand.end(), cardId), m_hand.end());
}