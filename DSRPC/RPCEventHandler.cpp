#include "RPCEventHandler.h"
namespace DSFramework {
	namespace DSRPC {
		RPCEventHandler::RPCEventHandler()
		{
		
		}

		RPCEventHandler::~RPCEventHandler()
		{
		}

		void RPCEventHandler::AddDeserializedEventHandler(std::shared_ptr<IDeserializedEventHandler> handler)
		{
			m_deserializedEventHandler.push_back(handler);
		}

		void RPCEventHandler::AddDispatchEventHandler(std::shared_ptr<IDispatchEventHandler> handler)
		{
			m_dispatchEventHandler.push_back(handler);

		}

		void RPCEventHandler::OnDeserialized(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			for (auto handler : m_deserializedEventHandler)
			{
				handler->OnDeserialized(session, request);
			}
		}

		void RPCEventHandler::OnDispatched(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			for (auto handler : m_dispatchEventHandler)
			{
				handler->OnDispatched(session, request);
			}
		}

		void RPCEventHandler::OnDispatchFailed(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			for (auto handler : m_dispatchEventHandler)
			{
				handler->OnDispatchFailed(session, request);
			}
		}

		void RPCEventHandler::AddCommitedEventHandler(std::shared_ptr<ICommitedEventHandler> handler)
		{
			m_commitedEventHandler.push_back(handler);
		}

		void RPCEventHandler::AddProcessedHandler(std::shared_ptr<IProcessedHandler> handler)
		{
			m_processedHandler.push_back(handler);
		}

		void RPCEventHandler::OnCommited(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			for (auto handler : m_commitedEventHandler)
			{
				handler->OnCommited(session, request);
			}
		}

		void RPCEventHandler::OnServiceNotFound(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			for (auto handler : m_commitedEventHandler)
			{
				handler->OnServiceNotFound(session, request);
			}
		}

		void RPCEventHandler::OnServiceParameterInvalid(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			for (auto handler : m_commitedEventHandler)
			{
				handler->OnServiceParameterInvalid(session, request);
			}
		}

		void RPCEventHandler::OnCompleted(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			for (auto handler : m_processedHandler)
			{
				handler->OnCompleted(session, request);
			}
		}

		void RPCEventHandler::OnFailed(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			for (auto handler : m_processedHandler)
			{
				handler->OnFailed(session, request);
			}
		}
	}
}