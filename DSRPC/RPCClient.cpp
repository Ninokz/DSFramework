#include "RPCClient.h"

namespace DSFramework {
	namespace DSRPC {
		RPCClient::RPCClient(std::string clientId, std::string clientName) :
			m_clientId(clientId),
			m_clientName(clientName),
			m_ioc(),
			m_timer(m_ioc),
			m_rpcThread()
		{
			ComponentInitialize();
			EventHandlerInitialize();
		}

		RPCClient::~RPCClient()
		{
			if (m_rpcThread.joinable()) {
				m_rpcThread.join();
			}
		}

		void RPCClient::RemoteCall(std::string ipaddress, short port, std::shared_ptr<RPCPacket> requestPacket, int timeoutSec)
		{
			m_rpcThread = std::thread([this, ipaddress, port,requestPacket, timeoutSec]() {
				m_timer.expires_after(std::chrono::seconds(timeoutSec));
				m_timer.async_wait(boost::bind(&RPCClient::StopCallProcedure, this, boost::asio::placeholders::error));
				CallProcedure(ipaddress, port, requestPacket);
				m_ioc.run();
			});
		}

		void RPCClient::ComponentInitialize()
		{
			m_rpcClientStub = std::make_shared<RPCClientStub>(m_clientId);
		}

		void RPCClient::EventHandlerInitialize()
		{
			m_client.AddDataEventHandler(std::static_pointer_cast<IDataEventHandler>(m_rpcClientStub));
		}

		bool RPCClient::CallProcedure(std::string ipaddress, short port ,std::shared_ptr<RPCPacket> requestPacket)
		{
			if (!requestPacket) {
				return false;
			}

			const char* data = nullptr;
			size_t size = 0;

			if (!Serialize(requestPacket, &data, &size)) {
				return false;
			}

			m_client.Connect(ipaddress, port);
			m_client.Send(data, static_cast<int>(size));
			delete[] data;
			data = nullptr;
			return true;
		}

		void RPCClient::StopCallProcedure(const boost::system::error_code& error)
		{
			m_client.Disconnect();
			m_ioc.stop();
		}

		inline bool RPCClient::Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size)
		{
			if (!packet || !data || !size) {
				return false;
			}

			*size = packet->ByteSizeLong();

			*data = new char[*size];
			if (!*data) {
				return false;
			}

			if (*size > INT32_MAX)
			{
				delete[] * data;
				*data = nullptr;
				*size = 0;
				return false;
			}

			if (!packet->SerializeToArray(const_cast<char*>(*data), static_cast<int>(*size))) 
			{
				delete[] * data;
				*data = nullptr;
				*size = 0;
				return false;
			}
			return true;
		}
	}
}