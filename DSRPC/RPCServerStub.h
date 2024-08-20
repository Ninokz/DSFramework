#pragma once
#include <string>
#include <memory>

#include "../DSCommunication/Session.h"
#include "../DSCommunication/DSCPacket.h"
#include "../DSCommunication/EventHandler.h"

#include "RequestDispatcher.h"
#include "ResponseDispatcher.h"
#include "RPCPacketSerializer.h"

using DSFramework::DSCommunication::Session;
using DSFramework::DSCommunication::DSCRecvPacket;
using DSFramework::DSCommunication::IDataEventHandler;

namespace DSFramework {
	namespace DSRPC {
		class RPCServerStub : public IDataEventHandler
		{
		private:
			std::shared_ptr<RequestDispatcher> m_RequestDispatcher;
			std::shared_ptr<ResponseDispatcher> m_ResponseDispatcher;
		public:
			virtual void OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg) override;
		};
	}
}