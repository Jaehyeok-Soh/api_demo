#include "pch.h"
#include "CKeyManager.h"

CKeyManager* CKeyManager::m_pInstance = nullptr;

CKeyManager::CKeyManager()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

CKeyManager::~CKeyManager()
{
}

void CKeyManager::Update()
{
	for (int i = 0; i < VK_MAX; i++)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
		{
			m_bKeyState[i] = !m_bKeyState[i];
		}

		if (!m_bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
		{
			m_bKeyState[i] = !m_bKeyState[i];
		}
	}
}

bool CKeyManager::Key_Pressing(int _iKey)
{
	if (GetAsyncKeyState(_iKey) & 0x8000)
	{
		return true;
	}

	return false;
}

bool CKeyManager::Key_Down(int _iKey)
{
	if (!m_bKeyState[_iKey] && GetAsyncKeyState(_iKey) & 0x8000)
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	return false;
}

bool CKeyManager::Key_Up(int _iKey)
{
	if (m_bKeyState[_iKey] && !GetAsyncKeyState(_iKey) & 0x8000)
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	return false;
}
