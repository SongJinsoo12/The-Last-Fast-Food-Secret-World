#include "Button.h"

// BaseButton
BaseButton::BaseButton(std::string imgId) : id(imgId), visible(true) {}
BaseButton::~BaseButton() {}
void BaseButton::SetVisible(bool v) { visible = v; }
bool BaseButton::IsVisible() const { return visible; }
std::string BaseButton::GetId() const { return id; }

// RectButton
RectButton::RectButton(std::string imgId, RECT r) : BaseButton(imgId), rect(r) {}
void RectButton::Draw() {
    if (!visible) return;
    m_rend.MoveImage(id, Gdiplus::Rect(
        rect.left, rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top));
}
bool RectButton::HitTest(int mx, int my) {
    return visible &&
        mx >= rect.left && mx <= rect.right &&
        my >= rect.top && my <= rect.bottom;
}

// CircleButton
CircleButton::CircleButton(std::string imgId, int x, int y, int r)
    : BaseButton(imgId), cx(x), cy(y), radius(r) {
}
void CircleButton::Draw() {
    if (!visible) return;
    m_rend.MoveImage(id, Gdiplus::Rect(cx - radius, cy - radius, radius * 2, radius * 2));
}
bool CircleButton::HitTest(int mx, int my) {
    int dx = mx - cx;
    int dy = my - cy;
    return visible && (dx * dx + dy * dy <= radius * radius);
}

// PolygonButton
PolygonButton::PolygonButton(std::string imgId, std::vector<POINT> pts)
    : BaseButton(imgId), points(pts) {
}
void PolygonButton::Draw() {
    if (!visible) return;
    int minX = points[0].x, minY = points[0].y;
    int maxX = points[0].x, maxY = points[0].y;
    for (auto& p : points) {
        if (p.x < minX) minX = p.x;
        if (p.y < minY) minY = p.y;
        if (p.x > maxX) maxX = p.x;
        if (p.y > maxY) maxY = p.y;
    }
    m_rend.MoveImage(id, Gdiplus::Rect(minX, minY, maxX - minX, maxY - minY));
}
bool PolygonButton::HitTest(int mx, int my) {
    bool inside = false;
    for (size_t i = 0, j = points.size() - 1; i < points.size(); j = i++) {
        if (((points[i].y > my) != (points[j].y > my)) &&
            (mx < (points[j].x - points[i].x) * (my - points[i].y) / (points[j].y - points[i].y) + points[i].x))
            inside = !inside;
    }
    return visible && inside;
}