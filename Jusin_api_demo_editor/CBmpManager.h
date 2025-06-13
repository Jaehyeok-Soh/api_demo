#pragma once
#include "CMyBmp.h"

class CBmpManager
{
private:
	CBmpManager();
	~CBmpManager();

public:
	HDC Find_Image(const TCHAR* pImgKey);
	HBITMAP Get_HBitmap(const TCHAR* pImgKey);

	void Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey);
	void Release();

public:
	static CBmpManager* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CBmpManager();
		}

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CBmpManager* m_pInstance;

	map<const TCHAR*, CMyBmp*> m_mapBit;
};

