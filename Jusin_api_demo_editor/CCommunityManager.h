#pragma once
class CCommunityManager
{
public:
	CCommunityManager();
	~CCommunityManager();

	CCommunityManager(CCommunityManager&) = delete;
	CCommunityManager(CCommunityManager&&) = delete;
	CCommunityManager& operator()(CCommunityManager&) = delete;
	CCommunityManager& operator()(CCommunityManager&&) = delete;

public:
	static CCommunityManager* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CCommunityManager();
		}

		return m_pInstance;
	}

	static void DestoryInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void Initialize();

	int OpenSocket();
	void SendSocket(string sendMsg);
	string ListenSocket();

	static void CloseSocket();

private:
	static CCommunityManager* m_pInstance;
	static SOCKET m_Csock;
};

