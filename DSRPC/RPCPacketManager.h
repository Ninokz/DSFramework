#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <shared_mutex>
#include <mutex>

#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "../DSCommunication/Logger.h"

#include "RPCPacket.pb.h"
#include "RPCEventHandler.h"

using DSFramework::DSComponent::Log;
using DSFramework::DSComponent::Logger;

using DSFramework::DSRPC::ICommitedEventHandler;
using DSFramework::DSRPC::IDeserializedEventHandler;
using DSFramework::DSRPC::IDispatchEventHandler;
using DSFramework::DSRPC::IProcessedEventHandler;
using DSFramework::DSRPC::Packet::RPCPacket;

namespace DSFramework {
	namespace DSRPC {
		class RPCPacketManager : 
			public IDeserializedEventHandler, 
			public IDispatchEventHandler, 
			public ICommitedEventHandler, 
			public IProcessedEventHandler,
			public IServiceEventHandler
		{
		private:
			using REQUEST_ID = std::string;
			using SESSION_ID = std::string;
		private:
			std::unordered_map<REQUEST_ID, std::pair<SESSION_ID, std::shared_ptr<Packet::RPCPacket>>> m_requests;
			std::shared_mutex m_requestsMutex;
		public:
			RPCPacketManager() = default;
			virtual ~RPCPacketManager() = default;

			int GetRequestCount();
		private:
			static inline std::string CurrentTime() { return boost::posix_time::to_simple_string(boost::posix_time::microsec_clock::local_time()); }

			void AddRequest(const std::string requestID, const std::string sessionID, std::shared_ptr<RPCPacket> request);

			void RemoveRequest(const std::string& requestID);

			void UpdateRequestStatus(const std::string& requestID, Packet::RPCPacketStatus status);

			std::shared_ptr<Packet::RPCPacket> QueryRequest(const std::string& requestID, bool* queryResult);

			std::string InitRPCPacket(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> packet);
		public:
			virtual void OnDeserialized(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;
			virtual void OnDispatched(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;
			virtual void OnDispatchFailed( const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;

			virtual void OnCommited(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;
			virtual void OnServiceNotFound(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;
			virtual void OnServiceParameterInvalid(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;
			virtual void OnServiceError(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;
			virtual void OnServiceEmptyRequest(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;

			virtual void OnCompleted(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;
			virtual void OnFailed(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> requestD) override;
		};
	}
}