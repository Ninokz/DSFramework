#include "RequestDispatcher.h"

namespace DSFramework {
	namespace DSRPC {
		RequestDispatcher::RequestDispatcher(size_t maxWaitedDispatch) : Dispatcher(maxWaitedDispatch)
		{
		}

		RequestDispatcher::~RequestDispatcher()
		{
		}

		bool RequestDispatcher::DispatchDSCMessage(std::shared_ptr<Packet::RPCPacket> request)
		{
			return false;
		}
		bool RequestDispatcher::PostRequestToQueue(std::shared_ptr<Packet::RPCPacket> request)
		{
			if (this->m_requestQueue->size() < m_maxWaitedDispatch)
			{
				this->m_requestQueue->Push(request);
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}