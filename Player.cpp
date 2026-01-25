#include "Player.h"
#include "Mob.h"
#include "AtkCard.h"
#include "Card.h"

#include <algorithm>
#include <random>
#include <sstream>

static int ClampInt(int v, int lo, int hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
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

bool Player::IsAlive() const { return m_hp > 0; }

void Player::BeginTurn(int currentTurn)
{
    m_turn = currentTurn;

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
        m_discard.push_back(id);
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
    if (!CanPlayCard())
        return false;

    int idx = ChooseCardIndex();
    if (idx < 0 || idx >= (int)m_hand.size())
        return false;

    const int cardId = m_hand[idx];

    // 손패에서 제거 -> 버림더미로
    m_hand.erase(m_hand.begin() + idx);
    m_discard.push_back(cardId);

    // 선택 인덱스 갱신
    m_selectedIndex = -1;

    // 실제 효과는 외부에서 처리
    if (resolver)
        resolver(cardId, *this, mob);

    // 플레이 카운트
    ++m_playsUsedThisTurn;
    m_playedThisTurn = (m_playsUsedThisTurn >= m_playLimitThisTurn);
    return true;

    // 바퀴벌레 발동 불가
    if (cardId == (int)CardId::Cockroach)
    {
        m_selectedIndex = -1;
        return false;
    }
}

void Player::AddDelayedHpLoss(int turnsLater, int healAmount)
{
    m_hasDelayedHpLoss = true;
    m_delayedHpLossTurnsLeft = turnsLater;

    // (heal + 25%) 버림
    m_delayedHpLossAmount = static_cast<int>(healAmount * 1.25f);
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

    // Damage는 int라고 가정
    atk.Damage = static_cast<int>(atk.Damage * m_nextAtkMultiplier);

    m_hasNextAtkMultiplier = false;
    m_nextAtkMultiplier = 1.0f;
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
