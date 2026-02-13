#pragma once
#include <vector>
#include <memory>
#include "Button.h"

class ButtonManager {
private:
    std::vector<std::shared_ptr<BaseButton>> buttons;

public:
    // 중복 ID 방지
    bool AddButton(std::shared_ptr<BaseButton> btn);

    void DrawAll();
    // 클릭된 버튼의 ID를 반환 (없으면 빈 문자열)
    std::string HandleClickId(int mx, int my);


    void SetVisibleAll(bool v);
    void SetVisibleById(const std::string& id, bool v);
    int GetButtonSize();
};

extern ButtonManager btnManager; // 전역 버튼 매니저
//extern ButtonManager btnManager;
