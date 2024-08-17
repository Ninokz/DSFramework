#include "IOServicePool.h"

namespace DSFramework {
	namespace DSComponent {
		IOServicePool::IOServicePool(std::size_t poolSize) : 
			m_ioContexts(poolSize),
			m_works(poolSize),
			m_nextIOContext(0)
		{
			for (size_t i = 0; i < poolSize; i++)
			{
				m_works[i] = std::unique_ptr<Work>(new Work(m_ioContexts[i]));
			}

			for (size_t i = 0; i < poolSize; i++)
			{
				m_threads.emplace_back([this, i]() {
					m_ioContexts[i].run();
				});
			}
		}

		IOServicePool::~IOServicePool()
		{	
			Stop();
			LOG_INFO_CONSOLE("IOServicePool destoried");
		}

		boost::asio::io_context& IOServicePool::getIOContext() {
			auto& context = m_ioContexts[m_nextIOContext++];
			if (m_nextIOContext == m_ioContexts.size())
			{
				m_nextIOContext = 0;
			}
			return context;
		}

		void IOServicePool::Stop() {
			for (auto& work : m_works)
			{
				work.reset();
			}

			for (auto& thread : m_threads)
			{
				thread.join();
			}
		}
	}
}