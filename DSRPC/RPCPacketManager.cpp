#include "RPCPacketManager.h"

namespace DSFramework {
	namespace DSRPC {
		int RPCPacketManager::GetRequestCount()
		{
			std::shared_lock<std::shared_mutex> lock(m_requestsMutex);
			return static_cast<int>(m_requests.size());
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
			auto it = m_requests.find(requestID);
			if (it != m_requests.end())
			{
				std::unique_lock<std::shared_mutex> lock(m_requestsMutex);
				it->second.second->set_status(status);

				switch (status)
				{
				case DSFramework::DSRPC::Packet::SUBMITTED:
					it->second.second->set_created_time(CurrentTime());
					break;
				case DSFramework::DSRPC::Packet::WAITING:
					it->second.second->set_post_time(CurrentTime());
					break;
				case DSFramework::DSRPC::Packet::COMMITED:
					it->second.second->set_commited_time(CurrentTime());
					break;
				case DSFramework::DSRPC::Packet::COMPLETED:
					it->second.second->set_completed_time(CurrentTime());
					break;
				case DSFramework::DSRPC::Packet::FAILED:
					it->second.second->set_failed_time(CurrentTime());
					break;
				default:
					break;
				}
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

		std::string RPCPacketManager::InitRPCPacket(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> packet)
		{
			std::string uid = boost::uuids::to_string(boost::uuids::random_generator()());
			packet->set_request_id(uid);
			packet->set_status(Packet::RPCPacketStatus::SUBMITTED);
			packet->set_error(Packet::RPCPacketError::PKT_NO_ERROR);
			packet->set_inner_id(session->GetUUID());
			packet->set_created_time(CurrentTime());
			packet->set_commited_time("");
			packet->set_completed_time("");
			return uid;
		}

		void RPCPacketManager::OnDeserialized(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			InitRPCPacket(session, request);
			AddRequest(request->request_id(), session->GetUUID(), request);
			LOG_INFO_CONSOLE("Request remain count: " + std::to_string(GetRequestCount()));
		}

		void RPCPacketManager::OnDispatched(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			UpdateRequestStatus(request->request_id(), Packet::RPCPacketStatus::WAITING);
		}

		void RPCPacketManager::OnDispatchFailed(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			const std::string& request_id = request->request_id();
			UpdateRequestStatus(request_id, Packet::RPCPacketStatus::FAILED);
			RemoveRequest(request_id);
		}

		void RPCPacketManager::OnCommited(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			UpdateRequestStatus(request->request_id(), Packet::RPCPacketStatus::COMMITED);
		}

		void RPCPacketManager::OnServiceNotFound(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			const std::string& request_id = request->request_id();
			UpdateRequestStatus(request->request_id(), Packet::RPCPacketStatus::FAILED);
			RemoveRequest(request_id);
		}

		void RPCPacketManager::OnServiceParameterInvalid(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			const std::string& request_id = request->request_id();
			UpdateRequestStatus(request->request_id(), Packet::RPCPacketStatus::FAILED);
			RemoveRequest(request_id);
		}

		void RPCPacketManager::OnServiceError(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			const std::string& request_id = request->request_id();
			UpdateRequestStatus(request->request_id(), Packet::RPCPacketStatus::FAILED);
			RemoveRequest(request_id);
		}

		void RPCPacketManager::OnServiceEmptyRequest(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			const std::string& request_id = request->request_id();
			UpdateRequestStatus(request->request_id(), Packet::RPCPacketStatus::FAILED);
			RemoveRequest(request_id);
		}

		void RPCPacketManager::OnCompleted(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			const std::string& request_id = request->request_id();
			UpdateRequestStatus(request_id, Packet::RPCPacketStatus::COMPLETED);
			RemoveRequest(request_id);
			LOG_INFO_CONSOLE("Request remain count: " + std::to_string(GetRequestCount()));
		}

		void RPCPacketManager::OnFailed(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			const std::string& request_id = request->request_id();
			UpdateRequestStatus(request_id, Packet::RPCPacketStatus::FAILED);
			RemoveRequest(request_id);
		}
	}
}