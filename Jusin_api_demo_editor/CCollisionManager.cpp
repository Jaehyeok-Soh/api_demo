#include "pch.h"
#include "CCollisionManager.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CObject.h"
#include "CComponent.h"

CCollisionManager* CCollisionManager::m_pInstance = nullptr;

CCollisionManager::CCollisionManager() : m_arrCheck{}
{

}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::Update()
{
    for (UINT iRow = 0; iRow < (UINT)OBJID::OBJ_END; ++iRow)
    {
        m_arrCheck[iRow];
        for (UINT iCol = iRow; iCol < (UINT)OBJID::OBJ_END; ++iCol) // iRow �׷�� �浹 üũ�� iCol �׷�
        {
            //��Ʈ����ũ�� ���� 1�� ����->���� �浹�ұ׷���.
            if (m_arrCheck[iRow] & (1 << iCol))
            {
                CollisionUpdateGroup((OBJID)iRow, (OBJID)iCol); // �ش� �׷쳢�� �浹 �˻�
            }
        }
    }
}

// �� �׷� ���� �浹 ó��
void CCollisionManager::CollisionUpdateGroup(OBJID _eLeft, OBJID _eRight)
{

    // ���� ������������ �� �׷��� ������Ʈ ����Ʈ ��������
    CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
    const list<CObject*>& listLeft = pCurScene->GetGroupObject(_eLeft);
    const list<CObject*>& listRight = pCurScene->GetGroupObject(_eRight);
    map<ULONGLONG, bool>::iterator iter;

    // ���� �׷��� ��� ������Ʈ ��ȸ
    for (auto leftObj : listLeft)
    {
        if (nullptr == leftObj || nullptr == leftObj->GetCollider() || listRight.empty())
            continue;

        for (auto rightObj : listRight)
        {
            // ������ �׷��� ��� ������Ʈ ��ȸ
            if (listRight.empty())
                break;

            if (nullptr == rightObj || nullptr == rightObj->GetCollider())
                continue;

            // �ڱ� �ڽŰ��� �浹�� ����
            if (leftObj == rightObj)
                continue;

            CCollider* pLeftCol = leftObj->GetCollider();
            CCollider* pRightCol = rightObj->GetCollider();

            // ���� �浹 ID ����
            COLIDER_ID ID;
            ID.Left_Id = pLeftCol->GetID();
            ID.Right_Id = pRightCol->GetID();

            iter = m_mapColInfo.find(ID.ID);

            // ó�� �浹 üũ�ϴ� ���̸� false�� �ʱ�ȭ
            if (iter == m_mapColInfo.end())
            {
                m_mapColInfo.insert(make_pair(ID.ID, false));
                iter = m_mapColInfo.find(ID.ID);
            }
            // �浹 �߻� ���� Ȯ��
            if (IsCollision(pLeftCol, pRightCol))
            {
                if (iter->second)
                {
                    // ���� �����ӿ����� �浹 ���̾���, ���ݵ� �浹 ��
                    if (leftObj->Get_Dead() || rightObj->Get_Dead())
                    {
                        // ��ü�� ���� �����̸� �浹 ���� ó��
                        pLeftCol->OnCollisionExit(pRightCol);
                        pRightCol->OnCollisionExit(pLeftCol);
                        iter->second = false;
                    }
                    else
                    {
                        // ��� �浹 ��
                        pLeftCol->OnCollision(pRightCol);
                        pRightCol->OnCollision(pLeftCol);
                    }
                }
                else
                {
                    // ���� �����ӿ��� �浹 �� �߰�, �̹� �����Ӻ��� �浹 ����
                    if (!leftObj->Get_Dead() && !rightObj->Get_Dead())
                    {
                        pLeftCol->OnCollisionEnter(pRightCol);
                        pRightCol->OnCollisionEnter(pLeftCol);
                        iter->second = true;
                    }
                }
            }
            else
            {
                // �̹� ������ �浹�� �ƴϰ�, ���� �����ӿ��� �浹�߾��ٸ� ���� ó��
                if (iter->second)
                {
                    pLeftCol->OnCollisionExit(pRightCol);
                    pRightCol->OnCollisionExit(pLeftCol);
                    iter->second = false;
                }
            }
        }
    }
}


// ���� �浹 ���θ� üũ
bool CCollisionManager::IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
{
    if (!(dynamic_cast<CComponent*>(_pLeftCol)->IsActive()) || !(dynamic_cast<CComponent*>(_pRightCol)->IsActive()))
        return false;

    Vec2 vLeftPos = _pLeftCol->GetFinalPos();
    Vec2 vLeftScale = _pLeftCol->GetScale();

    Vec2 vRightPos = _pRightCol->GetFinalPos();
    Vec2 vRightScale = _pRightCol->GetScale();

    if (abs(vRightPos.x - vLeftPos.x) <= (vLeftScale.x + vRightScale.x) / 2.f
        && abs(vRightPos.y - vLeftPos.y) <= (vLeftScale.y + vRightScale.y) / 2.f)
    {
        return true;
    }
    return false;
}

// �� �׷� ���� �浹 üũ ����
void CCollisionManager::CheckGroup(OBJID _eLeft, OBJID _eRight)
{

    UINT iRow = (UINT)_eLeft;
    UINT iCol = (UINT)_eRight;

    // �׻� iRow�� �۰� iCol�� ũ���� ����
    if (iCol < iRow)
    {
        // üũ ����
        iRow = (UINT)_eRight;
        iCol = (UINT)_eLeft;
    }

    // �浹 üũ ���� ����
    if (m_arrCheck[iRow] & (1 << iCol))
    {
        // üũ ����
        m_arrCheck[iRow] &= ~(1 << iCol);
    }
    else
    {
        // üũ ����
        m_arrCheck[iRow] |= (1 << iCol);
    }
}
