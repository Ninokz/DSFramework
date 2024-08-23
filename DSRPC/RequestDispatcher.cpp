#include "RequestDispatcher.h"


namespace DSFramework {
	namespace DSRPC {
		RequestDispatcher::RequestDispatcher(size_t maxWaitedDispatch, RPCEventHandler& m_rpcEventHandler) :
			Dispatcher(maxWaitedDispatch), m_rpcEventHandler(m_rpcEventHandler)
		{
			Start();
			LOG_INFO_CONSOLE("RequestDispatcher started");
		}

		RequestDispatcher::~RequestDispatcher()
		{
			LOG_INFO_CONSOLE("RequestDispatcher will stop and destory");
		}

		bool RequestDispatcher::PostRequestToQueue(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			std::string requester = sender->GetUUID();
			if (this->m_requestQueue->size() < this->m_maxWaitedDispatch)
			{
				/// 若成功分发请求则执行以下代码

				this->m_requestQueue->Push(std::make_pair(sender, dispatchItem));
				LOG_DEBUG_CONSOLE(requester + " request dispatched");
				m_rpcEventHandler.OnDispatched(sender, dispatchItem);
				return true;
			}
			else
			{
				/// 若分发请求失败则执行以下代码
				LOG_DEBUG_CONSOLE(requester + " request dispatched failed");
				m_rpcEventHandler.OnDispatchFailed(sender, dispatchItem);
				return false;
			}
		}

		void RequestDispatcher::DispatchDSCMessage(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			this->m_rpcEventHandler.OnCommited(sender, dispatchItem);

			std::string requester = sender->GetUUID();
			LOG_DEBUG_CONSOLE(requester + " request has commited");
		}


		void RequestDispatcher::OnDeserialized(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			this->PostRequestToQueue(session, request);
		}
	}
}