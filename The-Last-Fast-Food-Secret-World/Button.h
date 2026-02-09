#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include "RenderManager.h"

// 버튼 기본 클래스
class BaseButton {
protected:
    std::string id;   // RenderManager에 등록된 이미지 ID
    bool visible;

public:
    BaseButton(std::string imgId);
    virtual ~BaseButton();

    virtual void Draw() = 0;                  // 버튼 그리기
    virtual bool HitTest(int mx, int my) = 0; // 클릭 판정

    void SetVisible(bool v);
    bool IsVisible() const;
    std::string GetId() const;
};

// 사각형 버튼
class RectButton : public BaseButton {
private:
    RECT rect;

public:
    RectButton(std::string imgId, RECT r);
    void Draw() override;
    bool HitTest(int mx, int my) override;
};

// 원형 버튼
class CircleButton : public BaseButton {
private:
    int cx, cy, radius;

public:
    CircleButton(std::string imgId, int x, int y, int r);
    void Draw() override;
    bool HitTest(int mx, int my) override;
};

// 다각형 버튼
class PolygonButton : public BaseButton {
private:
    std::vector<POINT> points;

public:
    PolygonButton(std::string imgId, std::vector<POINT> pts);
    void Draw() override;
    bool HitTest(int mx, int my) override;
};