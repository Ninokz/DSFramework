#pragma once
#include <string>
#include <memory>

#include "../DSCommunication/Session.h"
#include "../DSCommunication/DSCPacket.h"
#include "../DSCommunication/EventHandler.h"

#include "RPCTaskManager.h"

using DSFramework::DSCommunication::Session;
using DSFramework::DSCommunication::DSCRecvPacket;
using DSFramework::DSCommunication::IDataEventHandler;
using DSFramework::DSRPC::RPCTaskManager;

namespace DSFramework {
	namespace DSRPC {
		class RPCServerStub : public IDataEventHandler
		{
		private:
		public:
			RPCServerStub();
			virtual ~RPCServerStub();

			virtual void OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg) override;
		};
	}
}