#pragma once
#ifdef __GNUG__
#include <cxxabi.h>
#endif

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
		class ServiceProcedure : public std::enable_shared_from_this<ServiceProcedure>
		{
		protected:
			const std::string m_serviceName;
		public:
			virtual bool Invoke(std::shared_ptr<RPCPacket> packet) = 0;
			virtual bool CheckParameters(std::shared_ptr<RPCPacket> packet) = 0;
		};
	}
}