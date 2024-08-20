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

#include "ConcurrentQueue.h"
#include "EventHandler.h"
#include "Logger.h"
#include "Session.h"

namespace DSFramework {
	namespace DSCommunication {
		class AsyncTcpClient
		{
		protected:
			boost::asio::io_context m_ioc;
			std::thread m_clientThread;
			std::shared_ptr<Session> m_session;

			tcp::endpoint m_remoteEp;
			std::shared_ptr<EventHandler> m_eventHandlerPtr;
			bool m_running;
		public:
			AsyncTcpClient();
			virtual ~AsyncTcpClient();
			bool Connect(std::string address, short port);
			void Disconnect();
			void Send(const char* data, int dataLength);

			void AddCloseEventHandler(std::shared_ptr<ICloseEventHandler> handler);
			void AddConnectEventHandler(std::shared_ptr<IConnectEventHandler> handler);
			void AddDataEventHandler(std::shared_ptr<IDataEventHandler> handler);
		};
	}
}

