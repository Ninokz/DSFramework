#pragma once
#include <string>
#include <memory>
#include <atomic>
#include <vector>
#include <unordered_map>

#include "../DSCommunication/Session.h"
#include "../DSCommunication/ThreadPool.h"
#include "../DSCommunication/Logger.h"

#include "RPCPacket.pb.h"
#include "RPCPacketFactory.h"
#include "RPCEventHandler.h"

using DSFramework::DSComponent::Log;
using DSFramework::DSComponent::Logger;

using DSFramework::DSComponent::ThreadPool;
using DSFramework::DSRPC::Packet::RPCPacket;
using DSFramework::DSCommunication::Session;

namespace DSFramework {
	namespace DSRPC {
		class RPCProcessor : public ICommitedEventHandler
		{
		public:
			using CheckFunction = std::function<bool(std::shared_ptr<RPCPacket>)>;
			using ExecuteFunction = std::function<void(std::shared_ptr<RPCPacket>)>;
		private:
			std::unordered_map<std::string, std::pair<CheckFunction, ExecuteFunction>> m_serviceProcedures;
			RPCEventHandler& m_rpcEventHandler;
		public:
			RPCProcessor(RPCEventHandler& rpcEventHandler);
			virtual ~RPCProcessor() = default;
			void RegisterService(std::string serviceName, CheckFunction checkFunction, ExecuteFunction executeFunction);
			virtual void OnCommited(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;
		private:
			bool CheckRequestService(std::shared_ptr<RPCPacket> packet);
			bool SearchService(std::string serviceName);
			bool CheckServiceParameter(std::string serviceName, std::shared_ptr<RPCPacket> packet);
			void Execute(std::string serviceName, std::shared_ptr<RPCPacket> packet, std::shared_ptr<Session> session);
		};
	}
}