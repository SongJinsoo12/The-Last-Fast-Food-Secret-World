#include "ButtonManager.h"

bool ButtonManager::AddButton(std::shared_ptr<BaseButton> btn) {
    for (auto& b : buttons) {
        if (b->GetId() == btn->GetId()) {
            return false; // 중복 ID → 추가 실패
        }
    }
    buttons.push_back(btn);
    return true;
}

void ButtonManager::DrawAll() {
    for (auto& btn : buttons) {
        btn->Draw();
    }
}

std::string ButtonManager::HandleClickId(int mx, int my) {
    for (auto& btn : buttons) {
        if (btn->HitTest(mx, my)) {
            return btn->GetId(); // 클릭된 버튼의 ID 반환
        }
    }
    return ""; // 클릭된 버튼 없음
}


void ButtonManager::SetVisibleAll(bool v) {
    for (auto& btn : buttons) {
        btn->SetVisible(v);
    }
}

void ButtonManager::SetVisibleById(const std::string& id, bool v) {
    for (auto& btn : buttons) {
        if (btn->GetId() == id) {
            btn->SetVisible(v);
            break;
        }
    }
}

int ButtonManager::GetButtonSize()
{
    return buttons.size();
}

// 전역 객체 정의
ButtonManager btnManager;