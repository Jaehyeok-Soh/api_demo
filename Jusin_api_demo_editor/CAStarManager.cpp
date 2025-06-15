#include "pch.h"
#include "CAStarManager.h"
#include "CTileManager.h"
#include "AStarNode.h"

CAStarManager* CAStarManager::m_pInstance = nullptr;

CAStarManager::CAStarManager()
{
	m_vecTile = CTileManager::Get_Instance()->GetTIleList();
	iCntX = CTileManager::Get_Instance()->GetCntX();
	iCntY = CTileManager::Get_Instance()->GetCntY();
	iMaxX = CTileManager::Get_Instance()->GetMaxX();
	iMaxY = CTileManager::Get_Instance()->GetMaxY();
}

CAStarManager::~CAStarManager()
{
}

// ------------------------------------------------------
// CAStarManager::FindPath   (Ÿ�� �ε��� �� Vec2 ������ǥ ���)
// ------------------------------------------------------
deque<Vec2> CAStarManager::FindPath(Vec2 start, Vec2 end)
{
    deque<Vec2> path;

    /* ---------- �޸���ƽ ---------- */
    auto heuristic = [](int ax, int ay, int bx, int by) {
        return static_cast<float>(abs(ax - bx) + abs(ay - by)); // ����ư
        };

    /* ---------- ��� ����� & ���� ���� ---------- */
    vector<unique_ptr<AStarNode>> storage;
    auto makeNode = [&](int x, int y, float g, float h, AStarNode* p) {
        storage.emplace_back(make_unique<AStarNode>(AStarNode{ x,y,g,h,p }));
        return storage.back().get();
        };

    /* ---------- �켱���� ť ---------- */
    auto cmp = [](AStarNode* a, AStarNode* b) { return a->f() > b->f(); };
    priority_queue<AStarNode*, vector<AStarNode*>, decltype(cmp)> open(cmp);

    /* ---------- gScore / closed ---------- */
    static float gScore[TILEY][TILEX];
    static bool  closed[TILEY][TILEX];
    for (int y = 0; y < TILEY; ++y)
        for (int x = 0; x < TILEX; ++x)
        {
            gScore[y][x] = FLT_MAX;
            closed[y][x] = false;
        }

    /* ---------- ���� ��� push ---------- */
    int sx = static_cast<int>(start.x);
    int sy = static_cast<int>(start.y);
    int ex = static_cast<int>(end.x);
    int ey = static_cast<int>(end.y);

    AStarNode* startNode = makeNode(sx, sy, 0.f,
        heuristic(sx, sy, ex, ey), nullptr);
    open.push(startNode);
    gScore[sy][sx] = 0.f;

    /* ---------- Ȯ�� ���� ---------- */
    auto tryPush = [&](int nx, int ny, AStarNode* parent)
        {
            if (nx < 0 || ny < 0 || nx >= TILEX || ny >= TILEY) return;
            if (!IsWalkable(nx, ny) || closed[ny][nx])          return;

            float newG = parent->g + 1.f;
            if (newG >= gScore[ny][nx]) return;   // �� ���� ��δ� ����

            gScore[ny][nx] = newG;
            float h = heuristic(nx, ny, ex, ey);
            open.push(makeNode(nx, ny, newG, h, parent));
        };

    /* ---------- ���� ���� ---------- */
    const AStarNode* goal = nullptr;

    while (!open.empty())
    {
        AStarNode* cur = open.top(); open.pop();

        if (cur->x == ex && cur->y == ey) { goal = cur; break; }

        closed[cur->y][cur->x] = true;

        tryPush(cur->x + 1, cur->y, cur);
        tryPush(cur->x - 1, cur->y, cur);
        tryPush(cur->x, cur->y + 1, cur);
        tryPush(cur->x, cur->y - 1, cur);
    }

    /* ---------- ������ ---------- */
    if (goal)
    {
        for (const AStarNode* p = goal; p; p = p->parent)
        {
            float worldX = p->x * TILECX + TILECX * 0.5f;
            float worldY = p->y * TILECY + TILECY * 0.5f;
            path.emplace_front(Vec2(worldX, worldY));
        }
    }

    return path;  // ��� ������ ��� ����
}

bool CAStarManager::IsWalkable(int _nx, int _ny)
{
	return static_cast<CTile*>(m_vecTile[_ny * TILEX + _nx])->GetDrawID() != TILETYPE::PEEK_DISABLE;
}
