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

		void RPCEventHandler::OnDeserialized(const std::string requestID, const std::string sessionID, std::shared_ptr<RPCPacket> request)
		{
			for (auto handler : m_deserializedEventHandler)
			{
				handler->OnDeserialized(requestID, sessionID, request);
			}
		}

		void RPCEventHandler::OnDispatched(const std::string& requestID)
		{
			for (auto handler : m_dispatchEventHandler)
			{
				handler->OnDispatched(requestID);
			}
		}

		void RPCEventHandler::OnDispatchFailed(const std::string& requestID)
		{
			for (auto handler : m_dispatchEventHandler)
			{
				handler->OnDispatchFailed(requestID);
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

		void RPCEventHandler::OnCommited(const std::string& requestID)
		{
			for (auto handler : m_commitedEventHandler)
			{
				handler->OnCommited(requestID);
			}
		}

		void RPCEventHandler::OnServiceNotFound(const std::string& requestID)
		{
			for (auto handler : m_commitedEventHandler)
			{
				handler->OnServiceNotFound(requestID);
			}
		}

		void RPCEventHandler::OnServiceParameterInvalid(const std::string& requestID)
		{
			for (auto handler : m_commitedEventHandler)
			{
				handler->OnServiceParameterInvalid(requestID);
			}
		}

		void RPCEventHandler::OnCompleted(const std::string& requestID)
		{
			for (auto handler : m_processedHandler)
			{
				handler->OnCompleted(requestID);
			}
		}

		void RPCEventHandler::OnFailed(const std::string& requestID)
		{
			for (auto handler : m_processedHandler)
			{
				handler->OnFailed(requestID);
			}
		}
	}
}