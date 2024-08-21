#pragma once
#include <string>
#include <memory>

#include "../DSCommunication/Session.h"
#include "../DSCommunication/DSCPacket.h"
#include "../DSCommunication/EventHandler.h"

#include "RPCPacketFactory.h"
#include "RPCPacketManager.h"

using DSFramework::DSCommunication::Session;
using DSFramework::DSCommunication::DSCRecvPacket;
using DSFramework::DSCommunication::IDataEventHandler;
using DSFramework::DSRPC::Packet::RPCPacketFactory;
using DSFramework::DSRPC::RPCPacketManager;

namespace DSFramework {
	namespace DSRPC {
		class RPCServerStub : public IDataEventHandler
		{
		private:
			std::string m_serverid;
			std::shared_ptr<RPCPacketManager> m_packetManager;
		public:
			RPCServerStub();
			virtual ~RPCServerStub();
			virtual void OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg) override;
		private:

		};
	}
}