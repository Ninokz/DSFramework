#include "ResponseDispatcher.h"



namespace DSFramework {
	namespace DSRPC {
		ResponseDispatcher::ResponseDispatcher(size_t maxWaitedDispatch) : Dispatcher(maxWaitedDispatch)
		{
		}

		ResponseDispatcher::~ResponseDispatcher()
		{
		}

		bool ResponseDispatcher::DispatchDSCMessage(std::shared_ptr<Packet::RPCPacket> request)
		{
			return false;
		}

		bool ResponseDispatcher::PostRequestToQueue(std::shared_ptr<Packet::RPCPacket> request)
		{
			return false;
		}
	}
}