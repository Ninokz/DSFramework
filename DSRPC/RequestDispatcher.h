#pragma once

#include "Dispatcher.h"
#include "RPCPacket.pb.h"

using DSFramework::DSRPC::Packet::RPCPacket;
namespace DSFramework {
	namespace DSRPC {

		class RequestDispatcher : public Dispatcher<Packet::RPCPacket>
		{
		public:
			RequestDispatcher(size_t maxWaitedDispatch);
			virtual ~RequestDispatcher();

			virtual bool DispatchDSCMessage(std::shared_ptr<Packet::RPCPacket> request);

			virtual bool PostRequestToQueue(std::shared_ptr<Packet::RPCPacket> request);
		};
	}
}