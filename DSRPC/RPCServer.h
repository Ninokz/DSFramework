#pragma once
#include <string>
#include <memory>
#include <atomic>
#include <vector>
#include <unordered_map>

#include "../DSCommunication/ThreadPool.h"

#include "RPCPacket.pb.h"
#include "ServiceProcedure.h"

using DSFramework::DSComponent::ThreadPool;
using DSFramework::DSRPC::Packet::RPCPacket;

namespace DSFramework {
	namespace DSRPC {

		class RPCServer
		{
		private:
			std::unordered_map<std::string, std::shared_ptr<ServiceProcedure>> m_serviceProcedures;
		public:
			RPCServer();
			virtual ~RPCServer();
		};
	}
}