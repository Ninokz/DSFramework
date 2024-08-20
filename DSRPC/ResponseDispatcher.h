#pragma once

#include "Dispatcher.h"
#include "RPCPacket.pb.h"

using DSFramework::DSRPC::Packet::RPCPacket;
namespace DSFramework {
	namespace DSRPC {

		class ResponseDispatcher : public Dispatcher<Packet::RPCPacket>
		{
		public:
			ResponseDispatcher(size_t maxWaitedDispatch);
			virtual ~ResponseDispatcher();

			virtual bool DispatchDSCMessage(std::shared_ptr<Packet::RPCPacket> request);

			virtual bool PostRequestToQueue(std::shared_ptr<Packet::RPCPacket> request);
		};
	}
}