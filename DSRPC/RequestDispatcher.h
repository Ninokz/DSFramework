#pragma once
#include <string>
#include <memory>

#include "Dispatcher.h"
#include "RPCPacket.pb.h"

using DSFramework::DSRPC::Packet::RPCPacket;
namespace DSFramework {
	namespace DSRPC {
		class RequestDispatcher : Dispatcher<Packet::RPCPacket>
		{
		private:

		public:
			RequestDispatcher(size_t maxWaitedDispatchCount);
			virtual ~RequestDispatcher();

			virtual bool PostRequestToQueue(std::shared_ptr<Packet::RPCPacket> request);

			virtual bool DispatchDSCMessage(std::shared_ptr<Packet::RPCPacket> request);
		};
	}
}