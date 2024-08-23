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

		void RPCServer::AddService(std::string serviceName, ParamsCheck check, Func func)
		{
			RegisterService(serviceName, check, func);
		}

		void RPCServer::ComponentInitialize()
		{
			m_rpcWorkers = std::make_shared<RPCProcessor>(m_rpcEventHandler);
			m_rpcResponseDispatcher = std::make_shared<ResponseDispatcher>(m_maxResponsePaddingCount);
			m_rpcRequestDispatcher = std::make_shared<RequestDispatcher>(m_maxRequestPaddingCount, m_rpcEventHandler);

			m_rpcRequestManager = std::make_shared<RPCPacketManager>();
			m_rpcServerStub = std::make_shared<RPCServerStub>(m_serverId, m_rpcEventHandler);
		}

		void RPCServer::EventHandlerInitialize()
		{
			m_rpcEventHandler.AddDeserializedEventHandler(std::static_pointer_cast<IDeserializedEventHandler>(m_rpcRequestDispatcher));
			m_rpcEventHandler.AddDispatchEventHandler(std::static_pointer_cast<IDispatchEventHandler>(m_rpcRequestManager));

			m_rpcEventHandler.AddCommitedEventHandler(std::static_pointer_cast<ICommitedEventHandler>(m_rpcRequestManager));
			m_rpcEventHandler.AddCommitedEventHandler(std::static_pointer_cast<ICommitedEventHandler>(m_rpcWorkers));

			m_rpcEventHandler.AddServiceEventHandler(std::static_pointer_cast<IServiceEventHandler>(m_rpcRequestManager));
			m_rpcEventHandler.AddProcessedEventHandler(std::static_pointer_cast<IProcessedEventHandler>(m_rpcRequestManager));

			m_rpcEventHandler.AddDeserializedEventHandler(std::static_pointer_cast<IDeserializedEventHandler>(m_rpcResponseDispatcher));
			m_rpcEventHandler.AddDeserializedFailedEventHandler(std::static_pointer_cast<IDeserializedFailedEventHandler>(m_rpcResponseDispatcher));
			m_rpcEventHandler.AddDispatchEventHandler(std::static_pointer_cast<IDispatchEventHandler>(m_rpcResponseDispatcher));
			m_rpcEventHandler.AddCommitedEventHandler(std::static_pointer_cast<ICommitedEventHandler>(m_rpcResponseDispatcher));
			m_rpcEventHandler.AddServiceEventHandler(std::static_pointer_cast<IServiceEventHandler>(m_rpcResponseDispatcher));
			m_rpcEventHandler.AddProcessedEventHandler(std::static_pointer_cast<IProcessedEventHandler>(m_rpcResponseDispatcher));

			m_server.AddDataEventHandler(std::static_pointer_cast<IDataEventHandler>(m_rpcServerStub));
		}

		void RPCServer::RegisterService(std::string serviceName, ParamsCheck check, Func func)
		{
			m_rpcWorkers->RegisterService(serviceName, check, func);
		}
	}
}