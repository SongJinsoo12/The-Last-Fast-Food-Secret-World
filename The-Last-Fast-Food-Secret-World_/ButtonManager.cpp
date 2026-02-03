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

BaseButton* ButtonManager::HandleClick(int mx, int my) {
    for (auto& btn : buttons) {
        if (btn->HitTest(mx, my)) {
            return btn.get();
        }
    }
    return nullptr;
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

// 전역 객체 정의
ButtonManager btnManager;