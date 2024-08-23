#pragma once
#include <memory>
#include <string>

#include "../DSCommunication/ConcurrentQueue.h"
#include "../DSCommunication/Logger.h"

using DSFramework::DSComponent::Log;
using DSFramework::DSComponent::Logger;
using DSFramework::DSComponent::ConcurrentQueue;
namespace DSFramework {
	namespace DSRPC {

		template<class TSender,class TDispatchItem>
		class Dispatcher
		{
		public:
			using SenderPtr = std::shared_ptr<TSender>;
			using DispatchItemPtr = std::shared_ptr<TDispatchItem>;
		protected:
			std::shared_ptr<ConcurrentQueue<std::pair<SenderPtr, DispatchItemPtr>>> m_requestQueue;
			size_t m_maxWaitedDispatch;
			std::thread m_dispatcherThread;
			bool isStopped;
		public:
			Dispatcher(size_t maxWaitedDispatch) : isStopped(false), 
				m_maxWaitedDispatch(maxWaitedDispatch)
			{
				m_requestQueue = std::make_shared<ConcurrentQueue<std::pair<SenderPtr, DispatchItemPtr>>>();
				Start();
			}

			virtual ~Dispatcher()
			{
				if (!isStopped) Stop();
			}

			virtual bool PostRequestToQueue(SenderPtr sender, DispatchItemPtr dispatchItem) = 0;

			virtual void DispatchDSCMessage(SenderPtr sender, DispatchItemPtr dispatchItem) = 0;	
		private:
			void Start()
			{
				m_dispatcherThread = std::thread([this] {
					while (!isStopped)
					{
						auto requset = m_requestQueue->WaitPop();
						if (isStopped || requset == nullptr || (requset->first == nullptr && requset->second == nullptr))
						{
							isStopped = false;
							break;
						}
						DispatchDSCMessage(requset->first, requset->second);
					}
					});
				LOG_INFO_CONSOLE("Dispatcher started");
			}

			void Stop()
			{
				isStopped = true;
				SenderPtr quit_1 = nullptr;
				DispatchItemPtr quit_2 = nullptr;
				std::pair<SenderPtr, DispatchItemPtr> quit_msg(quit_1, quit_2);
				this->m_requestQueue->Push(quit_msg);
				if (m_dispatcherThread.joinable())
				{
					m_dispatcherThread.join();
				}
				LOG_INFO_CONSOLE("Dispatcher stopped");
			}
		};
	}
}