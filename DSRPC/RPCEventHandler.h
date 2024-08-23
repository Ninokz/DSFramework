#pragma once
#include <string>
#include <memory>
#include <atomic>
#include <vector>

#include "../DSCommunication/Session.h"

#include "RPCPacket.pb.h"

using DSFramework::DSCommunication::Session;
using DSFramework::DSRPC::Packet::RPCPacket;
namespace DSFramework {
	namespace DSRPC {

		class IDeserializedEventHandler
		{
		public:
			virtual void OnDeserialized(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) = 0;
		};

		class IDispatchEventHandler
		{
		public:
			virtual void OnDispatched(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) = 0;
			virtual void OnDispatchFailed(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) = 0;
		};

		class ICommitedEventHandler
		{
		public:
			virtual void OnCommited(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) = 0;
			virtual void OnServiceNotFound(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) = 0;
			virtual void OnServiceParameterInvalid(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) = 0;
		};

		class IProcessedHandler 
		{
		public:
			virtual void OnCompleted(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) = 0;
			virtual void OnFailed(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) = 0;
		};

		class RPCEventHandler : public std::enable_shared_from_this<RPCEventHandler>
		{
		protected:
			std::vector<std::shared_ptr<IDeserializedEventHandler>> m_deserializedEventHandler;
			std::vector<std::shared_ptr<IDispatchEventHandler>> m_dispatchEventHandler;
			std::vector<std::shared_ptr<ICommitedEventHandler>> m_commitedEventHandler;
			std::vector<std::shared_ptr<IProcessedHandler>> m_processedHandler;
		public:
			RPCEventHandler();
			virtual ~RPCEventHandler();

			void AddDeserializedEventHandler(std::shared_ptr<IDeserializedEventHandler> handler);
			void AddDispatchEventHandler(std::shared_ptr<IDispatchEventHandler> handler);
			void AddCommitedEventHandler(std::shared_ptr<ICommitedEventHandler> handler);
			void AddProcessedHandler(std::shared_ptr<IProcessedHandler> handler);

			void OnDeserialized(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request);
			void OnDispatched(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request);
			void OnDispatchFailed(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request);

			void OnCommited(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request);
			void OnServiceNotFound(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request);
			void OnServiceParameterInvalid(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request);

			void OnCompleted(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request);
			void OnFailed(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request);
		};	
	}
}
