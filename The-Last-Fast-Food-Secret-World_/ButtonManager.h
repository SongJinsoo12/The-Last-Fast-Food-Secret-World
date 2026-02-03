#pragma once
<<<<<<< HEAD
#pragma once
=======
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
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
    BaseButton* HandleClick(int mx, int my);

    void SetVisibleAll(bool v);
    void SetVisibleById(const std::string& id, bool v);
};

extern ButtonManager btnManager; // 전역 버튼 매니저