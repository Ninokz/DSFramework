#include "RPCServer.h"

namespace DSFramework {
	namespace DSRPC {
		RPCServer::RPCServer(std::string serverid, std::string serverName, short port, size_t maxRqSize, size_t maxRsSize, size_t maxSendPadding) :
			m_serverId(serverid),
			m_serverName(serverName),
			m_port(port),
			m_maxRequestPaddingCount(maxRqSize),
			m_maxResponsePaddingCount(maxRsSize),
			m_maxSendPaddingQueueCount(maxSendPadding),
			m_server(port, m_maxSendPaddingQueueCount)
		{
			rpcEventHandler = RPCEventHandler();
			ComponentInitialize();
			EventHandlerInitialize();
		}

		RPCServer::~RPCServer()
		{
		}

		void RPCServer::Start()
		{
			m_server.Start();
		}

		void RPCServer::ComponentInitialize()
		{
			rpcWorkers = std::make_shared<RPCProcessor>();
			rpcRequestDispatcher = std::make_shared<RequestDispatcher>(m_maxRequestPaddingCount, rpcEventHandler);
			rpcResponseDispatcher = std::make_shared<ResponseDispatcher>(m_maxResponsePaddingCount);
			rpcRequestManager = std::make_shared<RPCPacketManager>();
			rpcServerStub = std::make_shared<RPCServerStub>(rpcEventHandler);
		}

		void RPCServer::EventHandlerInitialize()
		{
			rpcEventHandler.AddDeserializedEventHandler(std::static_pointer_cast<IDeserializedEventHandler>(rpcRequestDispatcher));
			rpcEventHandler.AddDispatchEventHandler(std::static_pointer_cast<IDispatchEventHandler>(rpcRequestManager));

			rpcEventHandler.AddCommitedEventHandler(std::static_pointer_cast<ICommitedEventHandler>(rpcRequestManager));
			rpcEventHandler.AddCommitedEventHandler(std::static_pointer_cast<ICommitedEventHandler>(rpcWorkers));

			rpcEventHandler.AddServiceEventHandler(std::static_pointer_cast<IServiceEventHandler>(rpcRequestManager));
			rpcEventHandler.AddProcessedEventHandler(std::static_pointer_cast<IProcessedEventHandler>(rpcRequestManager));

			rpcEventHandler.AddDeserializedEventHandler(std::static_pointer_cast<IDeserializedEventHandler>(rpcResponseDispatcher));
			rpcEventHandler.AddDeserializedFailedEventHandler(std::static_pointer_cast<IDeserializedFailedEventHandler>(rpcResponseDispatcher));
			rpcEventHandler.AddDispatchEventHandler(std::static_pointer_cast<IDispatchEventHandler>(rpcResponseDispatcher));
			rpcEventHandler.AddCommitedEventHandler(std::static_pointer_cast<ICommitedEventHandler>(rpcResponseDispatcher));
			rpcEventHandler.AddServiceEventHandler(std::static_pointer_cast<IServiceEventHandler>(rpcResponseDispatcher));
			rpcEventHandler.AddProcessedEventHandler(std::static_pointer_cast<IProcessedEventHandler>(rpcResponseDispatcher));

			m_server.AddDataEventHandler(std::static_pointer_cast<IDataEventHandler>(rpcServerStub));
		}

		void RPCServer::RegisterService(std::string serviceName, ParamsCheck check, Func func)
		{
			rpcWorkers->RegisterService(serviceName, check, func);
		}
	}
}