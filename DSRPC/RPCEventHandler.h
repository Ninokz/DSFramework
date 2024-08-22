#pragma once
#include <string>
#include <memory>
#include <atomic>
#include <vector>


#include "RPCPacket.pb.h"

using DSFramework::DSRPC::Packet::RPCPacket;
namespace DSFramework {
	namespace DSRPC {

		class IDeserializedEventHandler
		{
		public:
			virtual void OnDeserialized(const std::string requestID, const std::string sessionID, std::shared_ptr<RPCPacket> request) = 0;
		};

		class IDispatchEventHandler
		{
		public:
			virtual void OnDispatched(const std::string& requestID) = 0;
			virtual void OnDispatchFailed(const std::string& requestID) = 0;
		};

		class RPCEventHandler : public std::enable_shared_from_this<RPCEventHandler>
		{
		protected:
			std::vector<std::shared_ptr<IDeserializedEventHandler>> m_deserializedEventHandler;
			std::vector<std::shared_ptr<IDispatchEventHandler>> m_dispatchEventHandler;
		public:
			RPCEventHandler();
			virtual ~RPCEventHandler();

			void AddDeserializedEventHandler(std::shared_ptr<IDeserializedEventHandler> handler);
			void AddDispatchEventHandler(std::shared_ptr<IDispatchEventHandler> handler);

			void OnDeserialized(const std::string requestID, const std::string sessionID, std::shared_ptr<RPCPacket> request);
			void OnDispatched(const std::string& requestID);
			void OnDispatchFailed(const std::string& requestID);
		};	
	}
}
