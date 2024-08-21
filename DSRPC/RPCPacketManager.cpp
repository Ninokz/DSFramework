#include "RPCPacketManager.h"

namespace DSFramework {
	namespace DSRPC {
		RPCPacketManager::RPCPacketManager()
		{
		}

		RPCPacketManager::~RPCPacketManager()
		{
		}

		void RPCPacketManager::AddRequest(const std::string requestID, const std::string sessionID, std::shared_ptr<RPCPacket> request)
		{
			std::unique_lock<std::shared_mutex> lock(m_requestsMutex);
			m_requests[requestID] = std::make_pair(sessionID, request);
		}

		void RPCPacketManager::RemoveRequest(const std::string& requestID)
		{
			std::unique_lock<std::shared_mutex> lock(m_requestsMutex);
			m_requests.erase(requestID);
		}

		void RPCPacketManager::UpdateRequestStatus(const std::string& requestID, Packet::RPCPacketStatus status)
		{
			std::unique_lock<std::shared_mutex> lock(m_requestsMutex);
			auto it = m_requests.find(requestID);
			if (it != m_requests.end())
			{
				it->second.second->set_status(status);
				if ((status & Packet::RPCPacketStatus::COMMITED) == Packet::RPCPacketStatus::COMMITED)
					it->second.second->set_commited_time(CurrentTime());
				else if ((status & Packet::RPCPacketStatus::COMPLETED) == Packet::RPCPacketStatus::COMPLETED)
					it->second.second->set_completed_time(CurrentTime());
			}
		}

		std::shared_ptr<Packet::RPCPacket> RPCPacketManager::QueryRequest(const std::string& requestID, bool* queryResult)
		{
			std::shared_lock<std::shared_mutex> lock(m_requestsMutex);
			auto it = m_requests.find(requestID);
			if (it != m_requests.end())
			{
				*queryResult = true;
				return it->second.second;
			}
			*queryResult = false;
			return nullptr;
		}

		void RPCPacketManager::OnDeserialized(const std::string requestID, const std::string sessionID, std::shared_ptr<RPCPacket> request)
		{
			AddRequest(requestID, sessionID, request);
		}

		void RPCPacketManager::OnDispatched(const std::string& requestID)
		{
			UpdateRequestStatus(requestID, Packet::RPCPacketStatus::COMMITED);
		}

		void RPCPacketManager::OnDispatchFailed(const std::string& requestID)
		{
			UpdateRequestStatus(requestID, Packet::RPCPacketStatus::COMPLETED);
			RemoveRequest(requestID);
		}
	}
}