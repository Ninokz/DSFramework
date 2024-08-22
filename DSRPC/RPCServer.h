#pragma once
#include <string>
#include <memory>
#include <atomic>
#include <vector>
#include <unordered_map>

#include "../DSCommunication/Session.h"
#include "../DSCommunication/ThreadPool.h"

#include "RPCPacket.pb.h"
#include "ServiceProcedure.h"

using DSFramework::DSComponent::ThreadPool;
using DSFramework::DSRPC::Packet::RPCPacket;
using DSFramework::DSCommunication::Session;

namespace DSFramework {
	namespace DSRPC {
		class IRPCServer
		{
		public:
			virtual bool SearchService(std::string serviceName) = 0;
			virtual bool CheckServiceParameter(std::string serviceName, std::shared_ptr<RPCPacket> packet) = 0;
			virtual void Execute(std::string serviceName, std::shared_ptr<RPCPacket> packet, std::shared_ptr<RPCEventHandler> eventHandler) = 0;
		};

		class RPCServer : public IRPCServer
		{
		private:
			std::unordered_map<std::string, std::shared_ptr<ServiceProcedure>> m_serviceProcedures;
		public:
			RPCServer() = default;
			virtual ~RPCServer() = default;
		public:
			virtual bool SearchService(std::string serviceName) override;
			virtual bool CheckServiceParameter(std::string serviceName, std::shared_ptr<RPCPacket> packet) override;
			virtual void Execute(std::string serviceName, std::shared_ptr<RPCPacket> packet, std::shared_ptr<RPCEventHandler> eventHandler) override;
		};
	}
}