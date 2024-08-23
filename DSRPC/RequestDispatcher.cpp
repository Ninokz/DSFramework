#include "RequestDispatcher.h"


namespace DSFramework {
	namespace DSRPC {
		RequestDispatcher::RequestDispatcher(size_t maxWaitedDispatch, RPCEventHandler& m_rpcEventHandler) :
			Dispatcher(maxWaitedDispatch), m_rpcEventHandler(m_rpcEventHandler)
		{
			Start();
			LOG_INFO_CONSOLE("RequestDispatcher Started");
		}

		RequestDispatcher::~RequestDispatcher()
		{
			LOG_INFO_CONSOLE("RequestDispatcher will destory");
		}

		bool RequestDispatcher::PostRequestToQueue(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			if (this->m_requestQueue->size() < this->m_maxWaitedDispatch)
			{
				this->m_requestQueue->Push(std::make_pair(sender, dispatchItem));
				/// 若成功分发请求则执行以下代码
				LOG_DEBUG_CONSOLE("Request dispatched success");
				m_rpcEventHandler.OnDispatched(sender, dispatchItem);
				return true;
			}
			else
			{
				/// 若分发请求失败则执行以下代码
				LOG_DEBUG_CONSOLE("Request dispatched failed");
				m_rpcEventHandler.OnDispatchFailed(sender, dispatchItem);
				return false;
			}
		}

		void RequestDispatcher::DispatchDSCMessage(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			LOG_DEBUG_CONSOLE("Request Commited");
			this->m_rpcEventHandler.OnCommited(sender, dispatchItem);
		}


		void RequestDispatcher::OnDeserialized(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			this->PostRequestToQueue(session, request);
		}
	}
}