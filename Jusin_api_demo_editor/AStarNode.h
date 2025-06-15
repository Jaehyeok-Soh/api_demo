#pragma once
struct AStarNode
{
    int x, y;          // 타일 인덱스
    float g, h;        // 비용
    AStarNode* parent; // 부모 포인터
    float f() const { return g + h; }
    bool operator>(const AStarNode& o) const { return f() > o.f(); }
};

