#pragma once
#include <memory>
#include <string>

#include "../DSCommunication/Logger.h"
#include "../DSCommunication/ConcurrentQueue.h"

using DSFramework::DSComponent::Logger;
using DSFramework::DSComponent::Log;
using DSFramework::DSComponent::ConcurrentQueue;

namespace DSFramework {
	namespace DSRPC {

		template<class TDispatchItem>
		class Dispatcher
		{
		protected:
			std::shared_ptr<ConcurrentQueue<std::shared_ptr<TDispatchItem>>> m_requestQueue;
			size_t m_maxWaitedDispatch;
			std::thread m_dispatcherThread;
			bool isStopped;
		public:
			Dispatcher(size_t maxWaitedDispatch) : isStopped(false), m_maxWaitedDispatch(maxWaitedDispatch)
			{
				m_requestQueue = std::make_shared<ConcurrentQueue<std::shared_ptr<TDispatchItem>>>();
			}

			virtual ~Dispatcher()
			{
				if (!isStopped) Stop();
			}

			void Start() 
			{
				m_dispatcherThread = std::thread([this] {
					while (!isStopped)
					{
						auto requset = m_requestQueue->WaitPop();
						if (isStopped || requset == nullptr)
						{
							isStopped = false;
							break;
						}
						DispatchDSCMessage(*(requset));
					}
				});
				LOG_INFO_CONSOLE("Dispatcher started");
			}

			void Stop() 
			{
				isStopped = true;
				std::shared_ptr<TDispatchItem> quit_msg = nullptr;
				this->m_requestQueue->Push(quit_msg);
				if (m_dispatcherThread.joinable())
				{
					m_dispatcherThread.join();
				}
				LOG_INFO_CONSOLE("Dispatcher stopped");
			}

			virtual bool PostRequestToQueue(std::shared_ptr<TDispatchItem> request) = 0;

			virtual bool DispatchDSCMessage(std::shared_ptr<TDispatchItem> request) = 0;
		};
	}
}