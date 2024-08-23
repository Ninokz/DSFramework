#include "AsyncTcpServer.h"


namespace DSFramework {
	namespace DSCommunication {
		AsyncTcpServer::AsyncTcpServer(short port, size_t maxSessionCount)
			: m_ioc(),
			m_eventHandlerPtr(std::make_shared<EventHandler>()),
			m_port(port),
			m_running(false),
			m_sessionManager(std::make_shared<SessionManager>(maxSessionCount))
		{
			m_acceptor = std::make_unique<ConAcceptor>(m_ioc, port, m_eventHandlerPtr);	

			m_eventHandlerPtr->AddConnectEventHandler(std::static_pointer_cast<IConnectEventHandler>(m_sessionManager));
			m_eventHandlerPtr->AddCloseEventHandler(std::static_pointer_cast<ICloseEventHandler>(m_sessionManager));
		}

		AsyncTcpServer::~AsyncTcpServer()
		{
			if (m_running)
			{
				m_ioc.stop();
				m_serverThread.join();
			}
			LOG_INFO_CONSOLE("Server stopped");
		}

		void AsyncTcpServer::Start()
		{
			try {
				boost::asio::signal_set signals(m_ioc, SIGINT, SIGTERM);
				signals.async_wait([this](auto, auto) {
					m_running = false;
					m_ioc.stop();
				});

				m_serverThread = std::thread([this] {
					m_running = true;
					m_ioc.run();
				});
				LOG_INFO_CONSOLE("Server running at " + std::to_string(m_port));
				m_serverThread.join();
			}
			catch (std::exception& e) {
				LOG_ERROR_CONSOLE_DETAIL(e.what());
			}	
		}

		void AsyncTcpServer::AddCloseEventHandler(std::shared_ptr<ICloseEventHandler> handler)
		{
			m_eventHandlerPtr->AddCloseEventHandler(handler);
		}

		void AsyncTcpServer::AddConnectEventHandler(std::shared_ptr<IConnectEventHandler> handler)
		{
			m_eventHandlerPtr->AddConnectEventHandler(handler);
		}

		void AsyncTcpServer::AddDataEventHandler(std::shared_ptr<IDataEventHandler> handler)
		{
			m_eventHandlerPtr->AddDataEventHandler(handler);
		}
	}
}