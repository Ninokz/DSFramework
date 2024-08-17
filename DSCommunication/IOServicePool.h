#pragma once
#ifdef _MSC_VER
#define _WIN32_WINNT 0x0601
#endif

#include <vector>
#include <memory>

#include <boost/asio.hpp>

#include "Singleton.h"
#include "Logger.h"

namespace DSFramework {
	namespace DSComponent {
		class IOServicePool : public Singleton<IOServicePool>
		{
			friend Singleton<IOServicePool>;
		private:
			using Context = boost::asio::io_context;
			using Work = boost::asio::io_context::work;
			using WorkPtr = std::unique_ptr<boost::asio::io_context::work>;
		private:
			std::vector<Context> m_ioContexts;
			std::vector<WorkPtr> m_works;
			std::vector<std::thread> m_threads;
			std::size_t m_nextIOContext;
		private:
			IOServicePool(std::size_t poolSize = std::thread::hardware_concurrency());

			IOServicePool(const IOServicePool&) = delete;
			IOServicePool& operator=(const IOServicePool&) = delete;
			IOServicePool(IOServicePool&&) = delete;
		public:		
			virtual ~IOServicePool();

			Context& getIOContext();
			void Stop();
		};
	}
}