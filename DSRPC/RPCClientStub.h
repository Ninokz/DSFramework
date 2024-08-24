#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "../DSCommunication/Logger.h"
#include "../DSCommunication/Session.h"
#include "../DSCommunication/DSCPacket.h"
#include "../DSCommunication/AsyncTcpClient.h"
#include "../DSCommunication/EventHandler.h"

#include "RPCPacket.pb.h"
#include "RPCPacketFactory.h"
#include "RPCEventHandler.h"

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
		public:
			std::string m_clientid;
			std::shared_ptr<RPCPacket> m_cachedRequestPacket;
		private:
			RPCClientStub(const RPCClientStub& other) = delete;
			RPCClientStub& operator=(const RPCClientStub& other) = delete;

			std::shared_ptr<RPCPacket> Deserialize(const char* data, int datalength, bool* serializeResult);
		public:
			RPCClientStub(std::string clientid);
			virtual ~RPCClientStub();
			virtual void OnData(const std::shared_ptr<Session> session, const std::shared_ptr<DSCRecvPacket> packet) override;
		protected:
			virtual void HandleResponse(std::shared_ptr<RPCPacket> response);
		};
	}
}