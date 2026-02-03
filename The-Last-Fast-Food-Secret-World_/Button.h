<<<<<<< HEAD
ï»¿#pragma once
=======
#pragma once
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
#include <windows.h>
#include <string>
#include <vector>
#include "RenderManager.h"

<<<<<<< HEAD
#define type_Cirle(p_id, p_Rect) make_shared<CircleButton>(p_id, p_Rect)
#define type_Rect(p_id, p_x, p_y, p_r) make_shared<RectButton>(p_id, p_x, p_y, p_r)

// ë²„íŠ¼ ê¸°ë³¸ í´ëž˜ìŠ¤
class BaseButton {
protected:
    std::string id;   // RenderManagerì— ë“±ë¡ëœ ì´ë¯¸ì§€ ID
=======
// ¹öÆ° ±âº» Å¬·¡½º
class BaseButton {
protected:
    std::string id;   // RenderManager¿¡ µî·ÏµÈ ÀÌ¹ÌÁö ID
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
    bool visible;

public:
    BaseButton(std::string imgId);
    virtual ~BaseButton();

<<<<<<< HEAD
    virtual void Draw() = 0;                  // ë²„íŠ¼ ê·¸ë¦¬ê¸°
    virtual bool HitTest(int mx, int my) = 0; // í´ë¦­ íŒì •
=======
    virtual void Draw() = 0;                  // ¹öÆ° ±×¸®±â
    virtual bool HitTest(int mx, int my) = 0; // Å¬¸¯ ÆÇÁ¤
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f

    void SetVisible(bool v);
    bool IsVisible() const;
    std::string GetId() const;
};

<<<<<<< HEAD
// ì‚¬ê°í˜• ë²„íŠ¼
=======
// »ç°¢Çü ¹öÆ°
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
class RectButton : public BaseButton {
private:
    RECT rect;

public:
    RectButton(std::string imgId, RECT r);
    void Draw() override;
    bool HitTest(int mx, int my) override;
};

<<<<<<< HEAD
// ì›í˜• ë²„íŠ¼
=======
// ¿øÇü ¹öÆ°
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
class CircleButton : public BaseButton {
private:
    int cx, cy, radius;

public:
    CircleButton(std::string imgId, int x, int y, int r);
    void Draw() override;
    bool HitTest(int mx, int my) override;
};

<<<<<<< HEAD
// ë‹¤ê°í˜• ë²„íŠ¼
=======
// ´Ù°¢Çü ¹öÆ°
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
class PolygonButton : public BaseButton {
private:
    std::vector<POINT> points;

public:
    PolygonButton(std::string imgId, std::vector<POINT> pts);
    void Draw() override;
    bool HitTest(int mx, int my) override;
};