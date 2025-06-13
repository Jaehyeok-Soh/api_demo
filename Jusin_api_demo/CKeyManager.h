#pragma once
class CKeyManager
{
public:
	CKeyManager();
	~CKeyManager();

	CKeyManager(CKeyManager&) = delete;
	CKeyManager(CKeyManager&&) = delete;
	CKeyManager& operator()(CKeyManager&) = delete;
	CKeyManager& operator()(CKeyManager&&) = delete;

public:
	static CKeyManager* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CKeyManager();
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

public:
	void Update();
	bool Key_Pressing(int _iKey);
	bool Key_Down(int _iKey);
	bool Key_Up(int _iKey);

private:
	static CKeyManager* m_pInstance;
	bool m_bKeyState[VK_MAX];

};