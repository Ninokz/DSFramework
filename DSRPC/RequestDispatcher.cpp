#include "RequestDispatcher.h"


namespace DSFramework {
	namespace DSRPC {
		RequestDispatcher::RequestDispatcher(size_t maxWaitedDispatch, RPCEventHandler& rpcEventHandler) :
			Dispatcher(maxWaitedDispatch),
			m_rpcEventHandler(rpcEventHandler)
		{

		}

		RequestDispatcher::~RequestDispatcher()
		{
		}

		bool RequestDispatcher::PostRequestToQueue(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			if (this->m_requestQueue->size() < this->m_maxWaitedDispatch)
			{
				this->m_requestQueue->Push(std::make_pair(sender, dispatchItem));
				HandlePostSuccess(sender, dispatchItem);
				return true;
			}
			else
			{
				HandlePostFaile(sender, dispatchItem);
				return false;
			}
		}

		void RequestDispatcher::HandlePostSuccess(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			/// 若成功分发请求则执行以下代码
			LOG_DEBUG_CONSOLE("Request dispatched success");
			
		}

		void RequestDispatcher::HandlePostFaile(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			/// 若分发请求失败则执行以下代码
			LOG_DEBUG_CONSOLE("Request dispatched failed");

		}

		void RequestDispatcher::DispatchDSCMessage(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			/// 调用上层服务处理请求
		}

		void RequestDispatcher::HandleCommited(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			LOG_DEBUG_CONSOLE("Request commited");
			m_rpcEventHandler.OnCommited(sender, dispatchItem);
			
		}

		void RequestDispatcher::HandleServiceNotFound(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			LOG_DEBUG_CONSOLE("Request Service Not found");
			m_rpcEventHandler.OnServiceNotFound(sender, dispatchItem);

		}

		void RequestDispatcher::HandleServiceParameterInvalid(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			LOG_DEBUG_CONSOLE("Request Service Parameter invalid");
			m_rpcEventHandler.OnServiceParameterInvalid(sender, dispatchItem);
		}

		void RequestDispatcher::OnDeserialized(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			PostRequestToQueue(session, request);
		}
	}
}