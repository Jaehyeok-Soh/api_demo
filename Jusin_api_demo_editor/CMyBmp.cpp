#include "pch.h"
#include "CMyBmp.h"

CMyBmp::CMyBmp()
{
}

CMyBmp::~CMyBmp()
{
	Release();
}

void CMyBmp::Load_Bmp(const TCHAR* pFilePath)
{
	HDC hDC = GetDC(g_hWnd);

	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitMap = (HBITMAP)LoadImage(NULL,
		pFilePath,
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if (!m_hBitMap)
	{
		MessageBox(NULL, pFilePath, L"Image load fail!", MB_OK);
		return;
	}

	m_hOldMap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);
}

void CMyBmp::Release()
{
	SelectObject(m_hMemDC, m_hOldMap);
	DeleteObject(m_hBitMap);
	DeleteDC(m_hMemDC);
}
