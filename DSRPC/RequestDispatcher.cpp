#include "RequestDispatcher.h"


namespace DSFramework {
	namespace DSRPC {
		RequestDispatcher::RequestDispatcher(size_t maxWaitedDispatchCount) : Dispatcher<Packet::RPCPacket>(maxWaitedDispatchCount)
		{
		}

		RequestDispatcher::~RequestDispatcher()
		{
		}

		bool RequestDispatcher::PostRequestToQueue(std::shared_ptr<Packet::RPCPacket> request)
		{
			if (this->m_requestQueue->size() < this->m_maxWaitedDispatch)
			{
				this->m_requestQueue->Push(request);
				return true;
			}
			return false;
		}

		bool RequestDispatcher::DispatchDSCMessage(std::shared_ptr<Packet::RPCPacket> request)
		{
			return false;
		}
	}
}