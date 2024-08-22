#pragma once
#include <string>
#include <memory>
#include <atomic>
#include <vector>
#include <unordered_map>

#include "../DSCommunication/ThreadPool.h"

#include "RPCPacket.pb.h"
#include "RPCServiceProcedure.h"

using DSFramework::DSComponent::ThreadPool;
using DSFramework::DSRPC::Packet::RPCPacket;

namespace DSFramework {
	namespace DSRPC {
		class IServiceProvider {
		public:
			virtual bool SearchService(const std::string& serviceName) = 0;
			virtual bool CheckParams(std::shared_ptr<Packet::RPCPacket> packet) = 0;
		};

		class RPCServer
		{
		private:

		public:
			RPCServer();
			virtual ~RPCServer();
		};
	}
}