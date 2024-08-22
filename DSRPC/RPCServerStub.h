#pragma once
#include <string>
#include <memory>

#include "../DSCommunication/Session.h"
#include "../DSCommunication/DSCPacket.h"
#include "../DSCommunication/EventHandler.h"

#include "RPCPacketFactory.h"
#include "RPCPacketManager.h"
#include "RequestDispatcher.h"
#include "RPCEventHandler.h"

using DSFramework::DSCommunication::Session;
using DSFramework::DSCommunication::DSCRecvPacket;
using DSFramework::DSCommunication::IDataEventHandler;
using DSFramework::DSRPC::Packet::RPCPacket;
using DSFramework::DSRPC::RPCPacketFactory;
using DSFramework::DSRPC::RPCEventHandler;

namespace DSFramework {
	namespace DSRPC {
		class RPCServerStub : public IDataEventHandler
		{
		private:
			std::string m_serverid;
			std::shared_ptr<RPCEventHandler> m_rpcEventHandler;
		public:
			RPCServerStub();
			virtual ~RPCServerStub();
			virtual void OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg) override;

			inline void AddDeserializedEventHandler(std::shared_ptr<IDeserializedEventHandler> handler);
			inline void AddDispatchEventHandler(std::shared_ptr<IDispatchEventHandler> handler);
			inline void AddCommitedEventHandler(std::shared_ptr<ICommitedEventHandler> handler);
			inline void AddProcessedHandler(std::shared_ptr<IProcessedHandler> handler);
		private:
			void Send(std::shared_ptr<Session> sender, std::shared_ptr<Packet::RPCPacket> packet);

			inline std::shared_ptr<RPCPacket> Deserialize(const char* data, int datalength, bool* serializeResult);

			inline bool Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size);
		};
	}
}