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
#include "RPCEventHandler.h"
#include "RPCPacketFactory.h"
#include "ResponseDispatcher.h"

using DSFramework::DSComponent::Log;
using DSFramework::DSComponent::Logger;

using DSFramework::DSComponent::ThreadPool;
using DSFramework::DSRPC::Packet::RPCPacket;
using DSFramework::DSCommunication::Session;
using DSFramework::DSRPC::ResponseDispatcher;


namespace DSFramework {
	namespace DSRPC {
		class RPCServer
		{
		public:
			using CheckFunction = std::function<bool(std::shared_ptr<RPCPacket>)>;
			using ExecuteFunction = std::function<void(std::shared_ptr<RPCPacket>, RPCEventHandler&, std::shared_ptr<Session>)>;
		private:
			std::unordered_map<std::string, std::pair<CheckFunction, ExecuteFunction>> m_serviceProcedures;
			ResponseDispatcher& m_responseDispatcher;
		public:
			RPCServer(ResponseDispatcher& responseDispatcher);
			virtual ~RPCServer() = default;

			void RegisterService(std::string serviceName, CheckFunction checkFunction, ExecuteFunction executeFunction);
		public:
			virtual bool SearchService(std::string serviceName);
			virtual bool CheckServiceParameter(std::string serviceName, std::shared_ptr<RPCPacket> packet);
			virtual void Execute(std::string serviceName, std::shared_ptr<RPCPacket> packet, RPCEventHandler& eventHandler, std::shared_ptr<Session> session);
		private:
			void HandleCompleted(std::shared_ptr<Session> sender, std::shared_ptr<RPCPacket> packet, RPCEventHandler& eventHandler);

			void HandleFailed(std::shared_ptr<Session> sender, std::shared_ptr<RPCPacket> packet, RPCEventHandler& eventHandler);
		};
	}
}