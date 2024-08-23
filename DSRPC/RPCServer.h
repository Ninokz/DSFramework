#pragma once
#include <iostream>
#include <memory>
#include <string>

#include "../DSCommunication/AsyncTcpServer.h"

#include "RPCProcessor.h"
#include "RPCServerStub.h"
#include "RPCEventHandler.h"
#include "RequestDispatcher.h"
#include "ResponseDispatcher.h"
#include "RPCPacketManager.h"

namespace DSFramework {
	namespace DSRPC {
		class RPCServer
		{
			using RPCWorkersPtr = std::shared_ptr<RPCProcessor>;
			using RPCRequestDispatcherPtr = std::shared_ptr<RequestDispatcher>;
			using RPCResponseDispatcherPtr = std::shared_ptr<ResponseDispatcher>;
			using RPCRequestManagerPtr = std::shared_ptr<RPCPacketManager>;

			using RPCServerStubPtr = std::shared_ptr<RPCServerStub>;

			using ParamsCheck = RPCProcessor::CheckFunction;
			using Func = RPCProcessor::ExecuteFunction;
		protected:
			std::string m_serverId;
			std::string m_serverName;
			size_t m_maxRequestPaddingCount;
			size_t m_maxResponsePaddingCount;
			size_t m_maxSendPaddingQueueCount;
			short m_port;

			RPCEventHandler m_rpcEventHandler;

			RPCWorkersPtr m_rpcWorkers;
			RPCRequestDispatcherPtr m_rpcRequestDispatcher;
			RPCResponseDispatcherPtr m_rpcResponseDispatcher;
			RPCRequestManagerPtr m_rpcRequestManager;
			RPCServerStubPtr m_rpcServerStub;

			DSFramework::DSCommunication::AsyncTcpServer m_server;
		private:
			RPCServer() = delete;
			RPCServer(const RPCServer&) = delete;
			RPCServer& operator=(const RPCServer&) = delete;
		public:
			RPCServer(std::string serverid,std::string serverName,short port,size_t maxRqSize,size_t maxRsSize,size_t maxSendPadding);
			virtual ~RPCServer();
			void Start();
		private:
			void ComponentInitialize();
			void EventHandlerInitialize();
			void RegisterService(std::string serviceName, ParamsCheck check, Func func);
		};
	}
}