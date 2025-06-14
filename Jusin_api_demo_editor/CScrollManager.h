#pragma once
class CScrollManager
{
public:
	CScrollManager();
	~CScrollManager();

	CScrollManager(CScrollManager&) = delete;
	CScrollManager(CScrollManager&&) = delete;
	CScrollManager& operator()(CScrollManager&) = delete;
	CScrollManager& operator()(CScrollManager&&) = delete;

public:
	const float Get_ScrollX() const { return m_fScrollX; }
	void Set_ScrollX(float _fX);

	const float Get_ScrollY() const { return m_fScrollY; }
	void Set_ScrollY(float _fY);

	float		Get_ScrollXMax() { return m_fScrollXMax; }
	float		Get_ScrollYMax() { return m_fScrollYMax; }

	void	   Set_ScrollXMax(float _fX) {m_fScrollXMax = _fX;}
	void		Set_ScrollYMax(float _fY) { m_fScrollYMax = _fY;}

	void		Reset_Scroll() { m_fScrollX = 0.f; m_fScrollY = 0.f; }

public:
	static CScrollManager* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CScrollManager();
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
	static CScrollManager* m_pInstance;

	float m_fScrollX;
	float m_fScrollY;
	float m_fScrollXMax;
	float m_fScrollYMax;
};