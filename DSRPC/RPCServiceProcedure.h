#pragma once
#include <string>
#include <memory>
#include <atomic>
#include <vector>

#include "../DSCommunication/ThreadPool.h"

#include "RPCPacket.pb.h"
#include "RPCEventHandler.h"


using DSFramework::DSRPC::Packet::RPCPacket;
using DSFramework::DSRPC::RPCEventHandler;
namespace DSFramework {
	namespace DSRPC {
		class RPCServiceProcedure : public std::enable_shared_from_this<RPCServiceProcedure>
		{
		public:
			std::string m_procedureId;
			std::string m_serviceName;
		public:
			RPCServiceProcedure(){}
			virtual ~RPCServiceProcedure() = default;
			virtual void Procedure() = 0;
		};

		class RPCServiceChecker {
		public:
		public:
			RPCServiceChecker() = default;
			virtual ~RPCServiceChecker() = default;
			virtual bool Check(std::shared_ptr<RPCPacket> request) = 0;
		};
	}
}