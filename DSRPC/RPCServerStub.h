#pragma once
#include <string>
#include <memory>

#include "../DSCommunication/Session.h"
#include "../DSCommunication/DSCPacket.h"
#include "../DSCommunication/EventHandler.h"

#include "RequestDispatcher.h"
#include "ResponseDispatcher.h"
#include "RPCPacketSerializer.h"
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
			std::string m_serveruid;
			size_t m_maxWaitedDispatch;
			std::shared_ptr<RequestDispatcher> m_requestDispatcher;
			std::shared_ptr<ResponseDispatcher> m_responseDispatcher;
			std::shared_ptr<RPCTaskManager> m_taskManager;
		public:
			RPCServerStub(std::string serverid,size_t maxWaitedDispatch);
			virtual ~RPCServerStub();
			virtual void OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg) override;
		private:
			void HandleDeserializeFailed(std::shared_ptr<Session> sender);
			void HandlePostFailed(std::shared_ptr<Session> sender);
			void HandlePostSuccess(std::shared_ptr<Session> sender, std::shared_ptr<Packet::RPCPacket> packet);
		};
	}
}