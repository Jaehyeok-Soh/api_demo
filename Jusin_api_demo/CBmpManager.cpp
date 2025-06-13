#include "pch.h"
#include "CBmpManager.h"

CBmpManager* CBmpManager::m_pInstance = nullptr;

CBmpManager::CBmpManager()
{
}

CBmpManager::~CBmpManager()
{
	Release();
}

HDC CBmpManager::Find_Image(const TCHAR* pImgKey)
{
	//auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), tagFinder(pImgKey));
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), [&](const pair<const TCHAR* const, CMyBmp*>& MyPair) {
		return !lstrcmp(MyPair.first, pImgKey);
		});

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second->Get_MemDC();
}

HBITMAP CBmpManager::Get_HBitmap(const TCHAR* pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), tagFinder(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second->Get_BitMap();
}

void CBmpManager::Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey)
{
	//auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), tagFinder(pImgKey));
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), [&](const pair<const TCHAR* const, CMyBmp*>& MyPair) {
		return !lstrcmp(MyPair.first, pImgKey);
		});

	if (iter == m_mapBit.end())
	{
		CMyBmp* pMyBmp = new CMyBmp();
		pMyBmp->Load_Bmp(pFilePath);

		m_mapBit.insert({ pImgKey, pMyBmp });
	}
}

void CBmpManager::Release()
{
	for_each(m_mapBit.begin(), m_mapBit.end(), DeleteMap());
	m_mapBit.clear();
}
