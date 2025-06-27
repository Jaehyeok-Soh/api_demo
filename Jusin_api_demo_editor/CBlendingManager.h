#pragma once
class CBlendingManager
{
public:
	CBlendingManager();
	~CBlendingManager();

public:
	static CBlendingManager* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CBlendingManager();
		}
		return m_pInstance;
	}

	static void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void Initialize();
	void Render(HDC _dc, wstring _path, float x, float y, float cx, float cy);
	void Release();

private:
	void InitGDIPlus();
	void ShutDownGDIPlus();

private:
	static CBlendingManager* m_pInstance;
	static ULONG_PTR g_GdiToken;
};

