#include "AsyncTcpClient.h"

namespace DSFramework {
	namespace DSCommunication {
		AsyncTcpClient::AsyncTcpClient(): 
			m_ioc(),
			m_running(false)
		{
			m_session = std::make_shared<Session>(m_ioc, &(this->m_eventHandler), SEND_QUEUE_MAX_SIZE);
		}

		AsyncTcpClient::~AsyncTcpClient()
		{
			if (m_running)
			{
				Disconnect();
			}
			LOG_INFO_CONSOLE("Client stopped");
		}

		bool AsyncTcpClient::Connect(std::string address, short port)
		{
			try 
			{
				m_remoteEp = tcp::endpoint(boost::asio::ip::address::from_string(address), port);
				boost::system::error_code error = boost::asio::error::host_not_found;
				m_session->GetSocket().connect(m_remoteEp, error);
				if (error)
				{
					LOG_WARN_CONSOLE("Error while connecting: " + std::to_string(error.value()) + " " + error.message());
					return false;
				}
				else
				{
					m_session->Start();
					m_clientThread = std::thread([this] {
						m_running = true;
						m_ioc.run();
						});
					LOG_INFO_CONSOLE("Client connected to " + address + ":" + std::to_string(port));
					return true;
				}
			}
			catch (std::exception& e)
			{
				LOG_ERROR_CONSOLE_DETAIL(e.what());
				return false;
			}
		}

		void AsyncTcpClient::Disconnect()
		{
			m_running = false;
			m_session->Close();
			m_ioc.stop();
			if (m_clientThread.joinable())
				m_clientThread.join();
			LOG_INFO_CONSOLE("Client disconnected");
		}

		void AsyncTcpClient::Send(const char* data, int dataLength)
		{
			m_session->Send(data, dataLength);
		}

		void AsyncTcpClient::AddCloseEventHandler(std::shared_ptr<ICloseEventHandler> handler)
		{
			m_eventHandler.AddCloseEventHandler(handler);
		}

		void AsyncTcpClient::AddConnectEventHandler(std::shared_ptr<IConnectEventHandler> handler)
		{
			m_eventHandler.AddConnectEventHandler(handler);
		}

		void AsyncTcpClient::AddDataReceivedEventHandler(std::shared_ptr<IDataEventHandler> handler)
		{
			m_eventHandler.AddDataEventHandler(handler);
		}
	}
}