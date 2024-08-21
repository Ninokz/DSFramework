#include "RPCPacketManager.h"

namespace DSFramework {
	namespace DSRPC {
		RPCPacketManager::RPCPacketManager()
		{
		}

		RPCPacketManager::~RPCPacketManager()
		{
		}

		RPCPacketManager::REQUEST_ID RPCPacketManager::AddRequest(std::shared_ptr<Packet::RPCPacket> request)
		{
			std::string uid = boost::uuids::to_string(boost::uuids::random_generator()());
			std::unique_lock<std::shared_mutex> lock(m_requestsMutex);
			m_requests[uid] = request;
			return uid;
		}

		void RPCPacketManager::RemoveRequest(REQUEST_ID& requestID)
		{
			std::unique_lock<std::shared_mutex> lock(m_requestsMutex);
			m_requests.erase(requestID);
		}

		void RPCPacketManager::UpdateRequestStatus(REQUEST_ID& requestID, Packet::RPCPacketStatus status)
		{
			std::unique_lock<std::shared_mutex> lock(m_requestsMutex);
			auto it = m_requests.find(requestID);
			if (it != m_requests.end())
			{
				it->second->set_status(status);
			}
		}

		std::shared_ptr<Packet::RPCPacket> RPCPacketManager::QueryRequest(REQUEST_ID& requestID,bool* queryResult)
		{
			std::shared_lock<std::shared_mutex> lock(m_requestsMutex);
			auto it = m_requests.find(requestID);
			if (it != m_requests.end())
			{
				*queryResult = true;
				return it->second;
			}
			*queryResult = false;
			return std::make_shared<Packet::RPCPacket>();
		}
	}
}