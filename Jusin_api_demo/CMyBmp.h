#pragma once
class CMyBmp
{
public:
	CMyBmp();
	~CMyBmp();

public:
	HDC Get_MemDC() { return m_hMemDC; }
	HBITMAP	Get_BitMap() { return m_hBitMap; }

public:
	void Load_Bmp(const TCHAR* pFilePath);
	void Release();

private:
	HDC m_hMemDC;

	HBITMAP m_hBitMap;
	HBITMAP m_hOldMap;
};