#include "Player.h"
#include "Mob.h"
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

void Player::RegisterCardType(int cardId, CType type)
{
    CardTypeTable()[cardId] = type;
}

CType Player::ResolveCardTypeById(int cardId)
{
    auto& t = CardTypeTable();
    auto it = t.find(cardId);
    if (it != t.end())
        return it->second;
    return CType::E_Magic; // unknown -> treat as support (ignored)
}

void Player::RecordCardHistory(int cardId)
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

bool Player::HasRecentTwoSameCombatType() const
{
    if (m_recentCombatCount < 2)
        return false;
    return m_recentCombatTypes[0] == m_recentCombatTypes[1];
}

Player::Player() {}

Player::Player(const std::wstring& name, int maxHp)
{
    m_name = name;
    m_maxHp = (maxHp <= 1) ? 1 : maxHp;
    m_hp = m_maxHp;
}

int Player::GetHP() const { return m_hp; }

int Player::GetMaxHP() const
{
    return m_maxHp;
}

void Player::SetHP(int hp)
{
    m_hp = ClampInt(hp, 0, m_maxHp);
}

void Player::Damage(int dmg)
{
    if (dmg < 0) dmg = 0;
    lastDamageTaken = dmg;
    SetHP(m_hp - dmg);
    RemoveAllFromHand((int)CardId::Cockroach);
}

void Player::Heal(int amount)
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

int Player::GetShield() const
{
    return m_shield;
}

void Player::SetShield(int v)
{
    if (v < 0) v = 0;
    m_shield = v;
}

void Player::AddShield(int v)
{
    if (v <= 0) return;
    m_shield += v;
}

int Player::TakeDamage(int dmg)
{
    if (dmg <= 0) return 0;

    int absorbed = 0;
    if (m_shield > 0)
    {
        absorbed = (m_shield >= dmg) ? dmg : m_shield;
        m_shield -= absorbed;
        dmg -= absorbed;
    }

    if (dmg <= 0) return 0;

    // HP 감소는 기존 Damage()로 통일 (Roach 제거 등 부가효과 유지)
    int before = m_hp;
    Damage(dmg);
    return before - m_hp;
}

void Player::AddDot(int dmg, int ticks)
{
    if (dmg <= 0 || ticks <= 0) return;
    p_dots.push_back({ dmg, ticks });
}

void Player::TickDots()
{
    if (p_dots.empty()) return;

    int total = 0;

    for (size_t i = 0; i < p_dots.size();)
    {
        PlayerDotInfo& d = p_dots[i];
        if (d.ticks > 0)
        {
            total += d.dmg;
            d.ticks--;
        }

        if (d.ticks <= 0)
            p_dots.erase(p_dots.begin() + i);
        else
            ++i;
    }

    if (total > 0)
    {
        // DOT는 "실드 무시"로 하고 싶으면 Damage(total)만 호출
        // DOT도 실드에 막히게 하고 싶으면 TakeDamage(total) 호출
        Damage(total);
    }
}

bool Player::IsAlive() const { return m_hp > 0; }

void Player::BeginTurn(int currentTurn)
{
    m_turn = currentTurn;
    SetShield(0); // "다음 내 턴 시작 시 실드 사라짐" 규칙

    // ---- 플레이 횟수 초기화 ----
    m_playsUsedThisTurn = 0;
    m_playLimitThisTurn = 1;

    // 예정된 "n턴 후 플레이 제한 변경" 처리
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

    // 턴 시작 시 선택 초기화(권장)
    m_selectedIndex = -1;

    // ---- 드로우 처리 (드로우 금지 턴이면 스킵) ----
    if (m_noDrawTurns > 0)
    {
        --m_noDrawTurns;
    }
    else
    {
        if ((int)m_hand.size() < m_maxHandSize)
            DrawCards(1);
    }

    // ---- 지연 HP 감소 처리 ----
    if (m_hasDelayedHpLoss)
    {
        --m_delayedHpLossTurnsLeft;
        if (m_delayedHpLossTurnsLeft <= 0)
        {
            Damage(m_delayedHpLossAmount);
            m_hasDelayedHpLoss = false;
        }
    }

    // ---- 지연 힐 처리 ----
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

void Player::EndTurn()
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
    // 다른 턴 종료 처리들
}

bool Player::CanPlayCard() const
{
    if (!IsAlive()) return false;
    if (m_hand.empty()) return false;

    // 플레이 제한
    if (m_playsUsedThisTurn >= m_playLimitThisTurn) return false;

    // 플레이어는 선택 기반
    if (m_selectedIndex < 0 || m_selectedIndex >= (int)m_hand.size()) return false;
    return true;
}

void Player::SetDeck(const std::vector<int>& cardIds, bool shuffle)
{
    m_deck = cardIds;
    m_hand.clear();
    m_discard.clear();
    m_selectedIndex = -1;
    if (shuffle) ShuffleDeck();
}

void Player::ShuffleDeck()
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

void Player::DrawCards(int count)
{
    for (int i = 0; i < count; ++i)
    {
        if ((int)m_hand.size() >= m_maxHandSize)
            return;

        if (m_deck.empty())
        {
            // 덱이 비면 버림더미를 덱으로
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

void Player::DiscardHand()
{
    for (int id : m_hand)
    {
        m_discard.push_back(id);
        RecordCardHistory(id);
    }
    m_hand.clear();
    m_selectedIndex = -1;
}

void Player::SetSelectedCardIndex(int index)
{
    if (index < 0 || index >= (int)m_hand.size())
    {
        m_selectedIndex = -1;
        return;
    }
    m_selectedIndex = index;
}

int Player::ChooseCardIndex() const
{
    if (m_selectedIndex < 0 || m_selectedIndex >= (int)m_hand.size())
        return -1;
    return m_selectedIndex;
}

bool Player::TryPlayOneCard(Mob& mob, const CardResolver_PlayerVsMob& resolver)
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
        resolver(cardId, *this, mob);

    // Count play
    ++m_playsUsedThisTurn;
    m_playedThisTurn = (m_playsUsedThisTurn >= m_playLimitThisTurn);
    return true;
}

void Player::AddDelayedHpLoss(int turnsLater, int healAmount)
{
    m_hasDelayedHpLoss = true;
    m_delayedHpLossTurnsLeft = turnsLater;

    // (heal + 25%) 버림
    m_delayedHpLossAmount = static_cast<int>(healAmount * 1.25f);
    cout << "데미지를 입고 말거다." << endl;
}

void Player::AddDelayedHeal(int turnsLater, int amount)
{
    m_hasDelayedHeal = true;
    m_delayedHealTurnsLeft = turnsLater;
    m_delayedHealAmount = amount;
}

void Player::SchedulePlayLimit(int turnsLater, int playLimit)
{
    m_hasScheduledPlayLimit = true;
    m_scheduledPlayLimitTurnsLeft = turnsLater;
    m_scheduledPlayLimit = playLimit;
    cout << "3턴 뒤에 왔다" << endl;
}

void Player::AddExtraPlaysThisTurn(int extraPlays)
{
    if (extraPlays <= 0) return;
    m_playLimitThisTurn += extraPlays;
    if (m_playLimitThisTurn < 1) m_playLimitThisTurn = 1;
}

void Player::AddNoDrawTurns(int turns)
{
    if (turns <= 0) return;
    // 누적 (더 긴 쪽 우선)
    m_noDrawTurns = std::max(m_noDrawTurns, turns);
}

void Player::SetNextAtkMultiplier(float mult)
{
    if (mult <= 0.0f) return;
    m_hasNextAtkMultiplier = true;
    m_nextAtkMultiplier = mult;
}

bool Player::HasNextAtkMultiplier() const
{
    return m_hasNextAtkMultiplier;
}

void Player::ApplyNextAttackModifiers(AtkCard& atk)
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

bool Player::HasPoison() const { return m_hasPoison; }
void Player::SetPoison(bool on) { m_hasPoison = on; }

int Player::GetHandSize() const { return (int)m_hand.size(); }
int Player::GetDeckSize() const { return (int)m_deck.size(); }
int Player::GetDiscardSize() const { return (int)m_discard.size(); }

void Player::AddCardToHand(int cardId)
{
    if ((int)m_hand.size() >= m_maxHandSize)
    {
        // 손패가 가득이면 버림 더미로 
        m_discard.push_back(cardId);

        // 버림으로 가도 획득은 획득으므로 트리거 적용
        OnAcquireCard(cardId);
        return;
    }
    m_hand.push_back(cardId);

    // 손에 들어온 것도 획득이므로 트리거
    OnAcquireCard(cardId);
}

bool Player::PopTopDeckCard(int& outCardId)
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

bool Player::PopRandomHandCard(int& outCardId)
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

bool Player::DiscardHandCardAt(int index)
{
    if (index < 0 || index >= (int)m_hand.size()) return false;

    const int id = m_hand[index];
    m_hand.erase(m_hand.begin() + index);
    m_discard.push_back(id);

    if (m_selectedIndex == index) m_selectedIndex = -1;
    if (m_selectedIndex > index)  --m_selectedIndex;

    return true;
}

bool Player::DiscardRandomHandCards(int count)
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

bool Player::PeekTopDeck(int count, std::vector<int>& out) const
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

bool Player::ReorderTopDeck(const std::vector<int>& newOrder)
{
    const int n = (int)newOrder.size();
    if (n <= 0) return false;
    if ((int)m_deck.size() < n) return false;

    // 기존 top n 제거
    for (int i = 0; i < n; ++i)
        m_deck.pop_back();

    // newOrder[0]가 top이 되도록 역순 push
    for (int i = n - 1; i >= 0; --i)
        m_deck.push_back(newOrder[i]);

    return true;
}

void Player::ReturnHandToDeck(bool shuffleAfter)
{
    for (int id : m_hand)
        m_deck.push_back(id);
    m_hand.clear();
    m_selectedIndex = -1;

    if (shuffleAfter) ShuffleDeck();
}

// ----------------- WinAPI Drawing -----------------

void Player::Draw(HDC hdc, const RECT& area) const
{
    DrawPanel(hdc, area);

    // 손패 표시 영역(패널 아래쪽)
    RECT handArea = area;
    handArea.top = area.bottom - 40;
    DrawHand(hdc, handArea);
}

void Player::DrawPanel(HDC hdc, const RECT& area) const
{
    Rectangle(hdc, area.left, area.top, area.right, area.bottom);
    SetBkMode(hdc, TRANSPARENT);

    // 이름
    RECT nameRc = area;
    nameRc.left += 8;
    nameRc.top += 6;
    nameRc.bottom = nameRc.top + 20;
    DrawTextW(hdc, m_name.c_str(), -1, &nameRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // HP 텍스트
    std::wstringstream ss;
    ss << L"HP: " << m_hp << L"/" << m_maxHp;

    RECT hpTextRc = area;
    hpTextRc.left += 8;
    hpTextRc.top += 28;
    hpTextRc.bottom = hpTextRc.top + 20;
    const std::wstring hpText = ss.str();
    DrawTextW(hdc, hpText.c_str(), -1, &hpTextRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // 턴 텍스트
    std::wstringstream ts;
    ts << L"Turn: " << m_turn << L" (" << m_playsUsedThisTurn << L"/" << m_playLimitThisTurn << L")";
    RECT turnRc = area;
    turnRc.left += 8;
    turnRc.top += 48;
    turnRc.bottom = turnRc.top + 20;
    const std::wstring turnText = ts.str();
    DrawTextW(hdc, turnText.c_str(), -1, &turnRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // HP 바
    RECT barRc = area;
    barRc.left += 8;
    barRc.right -= 8;
    barRc.top = area.top + 72;
    barRc.bottom = barRc.top + 14;
    DrawHPBar(hdc, barRc);
}

void Player::DrawHPBar(HDC hdc, const RECT& barArea) const
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

void Player::DrawHand(HDC hdc, const RECT& area) const
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
void Player::AddCardToDeck(int cardId, bool triggerAcuire)
{
    m_deck.push_back(cardId);
    if (triggerAcuire)
    {
        OnAcquireCard(cardId);
    }
}

void Player::AddHealToDamageTurns(int turns)
{
    if (turns <= 0)
    {
        return;
    }
    m_healToDamageTurnsLeft = std::max(m_healToDamageTurnsLeft, turns);

}

bool Player::HasHealToDamageDebuff() const
{
    return m_healToDamageTurnsLeft > 0;
}

void Player::AddDisarrayTurns(int turns)
{
    if (turns <= 0)
    {
        return;
    }
    m_disarrayTurnsLeft = std::max(m_disarrayTurnsLeft, turns);
}

bool Player::IsDisarray() const
{
    return m_disarrayTurnsLeft > 0;
}

int Player::GetDisarrayTurnLeft() const
{
    return m_disarrayTurnsLeft;
}

void Player::ReturnHandToDeckAndRedraw(bool shuffleAfter)
{
    const int n = GetHandSize();
    ReturnHandToDeck(shuffleAfter);
    DrawCards(n);
}

void Player::TriggerScheduledEffectsNow()
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

void Player::SetNextAtkAttribute(CAttribute attr, CAttribute neutral)
{
    if (attr == neutral) return;
    m_hasNextAtkAttributeOverride = true;
    m_nextAtkAttributeOverride = attr;
}

void Player::SetNextAtkAttributeRandom(CAttribute neutral)
{
    std::vector<CAttribute> pool = { E_BULGOGI, E_SOURCE, E_CHESSE, E_VEGAT, E_BREAD };
    pool.erase(std::remove(pool.begin(), pool.end(), neutral), pool.end());
    if (pool.empty()) return;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, (int)pool.size() - 1);

    SetNextAtkAttribute(pool[dist(gen)], neutral);
}

bool Player::HasNextAtkAttributeOverride() const
{
    return m_hasNextAtkAttributeOverride;
}

void Player::ProhibitPlay(int turns)
{
    if (turns <= 0) return;
    m_playProhibitTurnsLeft = std::max(m_playProhibitTurnsLeft, turns);
}

void Player::ProhibitAttackPlay(int turns)
{
    if (turns <= 0) return;
    m_attackProhibitTurnsLeft = std::max(m_attackProhibitTurnsLeft, turns);
}

bool Player::IsPlayProhibited() const
{
    return m_playProhibitTurnsLeft > 0;
}

bool Player::IsAttackPlayProhibited() const
{
    return m_attackProhibitTurnsLeft > 0;
}

void Player::OnAcquireCard(int cardId)
{
    if (cardId == (int)CardId::Cockroach)
    {
        // 바퀴벌레 획득하면 덱에 추가
        // 무한 증식 방지
        AddCardToDeck((int)CardId::Cockroach, false);
    }
}

void Player::RemoveAllFromHand(int cardId)
{
    m_hand.erase(std::remove(m_hand.begin(), m_hand.end(), cardId), m_hand.end());
}