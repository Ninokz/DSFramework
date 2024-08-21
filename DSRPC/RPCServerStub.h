#pragma once
#include <string>
#include <memory>

#include "../DSCommunication/Session.h"
#include "../DSCommunication/DSCPacket.h"
#include "../DSCommunication/EventHandler.h"

#include "RPCPacketFactory.h"
#include "RPCPacketManager.h"
#include "RequestDispatcher.h"

using DSFramework::DSCommunication::Session;
using DSFramework::DSCommunication::DSCRecvPacket;
using DSFramework::DSCommunication::IDataEventHandler;
using DSFramework::DSRPC::Packet::RPCPacket;

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
			void Send(std::shared_ptr<Session> sender, std::shared_ptr<Packet::RPCPacket> packet);

			inline std::shared_ptr<RPCPacket> Deserialize(const char* data, int datalength, bool* serializeResult);
			inline bool Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size);
		};
	}
}