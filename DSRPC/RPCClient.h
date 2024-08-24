#pragma once
#include <iostream>
#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include "../DSCommunication/AsyncTcpClient.h"

#include "RPCClientStub.h"

namespace DSFramework {
	namespace DSRPC {
		class RPCClient
		{
			using RPCClientStubPtr = std::shared_ptr<RPCClientStub>;
		private:
			std::string m_clientId;
			std::string m_clientName;

			std::thread m_rpcThread;
			boost::asio::io_context m_ioc;
			boost::asio::steady_timer m_timer;

			RPCClientStubPtr m_rpcClientStub;
			AsyncTcpClient m_client;
		public:
			RPCClient(std::string clientId, std::string clientName);
			virtual ~RPCClient();

			void RemoteCall(std::string ipaddress, short port, std::shared_ptr<RPCPacket> requestPacket, int timeoutSec);
		private:
			void ComponentInitialize();
			void EventHandlerInitialize();

			bool CallProcedure(std::string ipaddress, short port, std::shared_ptr<RPCPacket> requestPacket);
			void StopCallProcedure(const boost::system::error_code& error);

			inline bool Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size);
		};
	}
}