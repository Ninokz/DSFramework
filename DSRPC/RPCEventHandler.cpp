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
	}
}