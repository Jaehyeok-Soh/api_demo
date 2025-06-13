#pragma once
class CTimeManager
{
private:
	CTimeManager();
	~CTimeManager();

private:
	LARGE_INTEGER m_llPrevCount;
	LARGE_INTEGER m_llFrequency;
	LARGE_INTEGER m_llCurCount;
	// FPS
	// 1 �����Ӵ� �ð� Delta Time
	double m_dDT; // ������ ������ �ð��� 
	double m_dAcc; // 1�� üũ�� ���� ���� �ð�
	UINT  m_iCallCount;  // �Լ� ȣ�� Ƚ�� üũ
	UINT m_iFPS; // �ʴ� ȣ�� Ƚ��

public:
	void Initialize();
	void Update();
	void Render();
public:
	double GetDT() { return m_dDT; }
	float GetfDT() { return (float)m_dDT; }

public:
	static CTimeManager* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTimeManager;
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
	static CTimeManager* m_pInstance;
};

