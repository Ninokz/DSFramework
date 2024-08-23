#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include "../DSCommunication/Logger.h"
#include "../DSCommunication/Session.h"
#include "../DSCommunication/DSCPacket.h"
#include "../DSCommunication/AsyncTcpClient.h"
#include "../DSCommunication/EventHandler.h"

#include "RPCPacket.pb.h"
#include "RPCPacketFactory.h"

using DSFramework::DSComponent::Log;
using DSFramework::DSComponent::Logger;
using DSFramework::DSCommunication::Session;
using DSFramework::DSCommunication::DSCRecvPacket;
using DSFramework::DSCommunication::AsyncTcpClient;
using DSFramework::DSCommunication::IDataEventHandler;

using DSFramework::DSRPC::Packet::RPCPacket;
using DSFramework::DSRPC::RPCPacketFactory;

namespace DSFramework {
	namespace DSRPC {
		class RPCClientStub : public IDataEventHandler
		{
		private:
			std::string m_clientid;
		private:
			RPCClientStub(const RPCClientStub& other) = delete;
			RPCClientStub& operator=(const RPCClientStub& other) = delete;
		public:
			RPCClientStub(std::string clientid);
			virtual ~RPCClientStub();

			virtual void OnData(std::shared_ptr<Session> session, std::shared_ptr<DSCRecvPacket> packet) override;
		private:
			std::shared_ptr<RPCPacket> Deserialize(const char* data, int datalength, bool* serializeResult);
		};
	}
}