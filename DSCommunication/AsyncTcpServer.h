#pragma once
#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <mutex>
#include <memory>
#include <csignal>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include "ConAcceptor.h"
#include "SessionManager.h"
#include "ConcurrentQueue.h"
#include "EventHandler.h"
#include "Logger.h"

using DSFramework::DSComponent::Logger;
using DSFramework::DSComponent::Log;

namespace DSFramework {
	namespace DSCommunication {
		class AsyncTcpServer
		{
		protected:
			boost::asio::io_context m_ioc;
			std::thread m_serverThread;
			std::unique_ptr<ConAcceptor> m_acceptor;
			std::shared_ptr<SessionManager> m_sessionManager;
			EventHandler m_eventHandler;
			short m_port;

			bool m_running;
		private:
			AsyncTcpServer(const AsyncTcpServer& other) = delete;
			AsyncTcpServer& operator=(const AsyncTcpServer& other) = delete;
		public:
			AsyncTcpServer(short port, size_t maxSessionCount);
			virtual ~AsyncTcpServer();
			void Start();

			void AddCloseEventHandler(std::shared_ptr<ICloseEventHandler> handler);
			void AddConnectEventHandler(std::shared_ptr<IConnectEventHandler> handler);
			void AddDataReceivedEventHandler(std::shared_ptr<IDataEventHandler> handler);
		};
	}
}
