#include "SessionManager.h"


namespace DSFramework {
	namespace DSCommunication {
		SessionManager::SessionManager(size_t maxSize) : m_maxSessionCount(maxSize)
		{

		}

		SessionManager::~SessionManager() 
		{
			
		}

		bool SessionManager::AddSession(std::shared_ptr<Session> session)
		{
			if (m_sessions.size() >= m_maxSessionCount)
			{
				session->Close();
				return false;
			}
			m_sessions.insert(std::make_pair(session->GetUUID(), session));
			return true;
		}

		void SessionManager::RemoveSession(std::string& uuid)
		{
			m_sessions.erase(uuid);
			LOG_INFO_CONSOLE("Session" + uuid + " is removed.");
		}

		void SessionManager::OnClose(std::shared_ptr<Session> sender)
		{
			std::string uuid = sender->GetUUID();
			RemoveSession(uuid);
		}

		void SessionManager::OnConnect(std::shared_ptr<Session> sender)
		{
			if (AddSession(sender))
			{
				LOG_INFO_CONSOLE("Session " + sender->GetUUID() + " is connected.");
			}
			else
			{
				LOG_WARN_CONSOLE("Session " + sender->GetUUID() + " is rejected, Sessions is full.");
			}
		}
	}
}