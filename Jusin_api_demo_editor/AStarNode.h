#pragma once
struct AStarNode
{
    int x, y;          // Ÿ�� �ε���
    float g, h;        // ���
    AStarNode* parent; // �θ� ������
    float f() const { return g + h; }
    bool operator>(const AStarNode& o) const { return f() > o.f(); }
};

