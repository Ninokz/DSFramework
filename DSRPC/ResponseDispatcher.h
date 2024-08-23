#pragma once
#include <string>
#include <memory>

#include "../DSCommunication/Session.h"

#include "Dispatcher.h"
#include "RPCPacket.pb.h"
#include "RPCPacketFactory.h"
#include "RPCEventHandler.h"

using DSFramework::DSRPC::RPCPacketFactory;
using DSFramework::DSCommunication::Session;
using DSFramework::DSRPC::Packet::RPCPacket;
namespace DSFramework {
	namespace DSRPC {
		class ResponseDispatcher : public Dispatcher<Session, RPCPacket>, 
			public IDeserializedEventHandler,
			public IDispatchEventHandler,
			public ICommitedEventHandler,
			public IProcessedEventHandler,
			public IServiceEventHandler,
			public IDeserializedFailedEventHandler
		{
		public:
			ResponseDispatcher(size_t maxWaitedDispatch);
			virtual ~ResponseDispatcher();
		private:
			virtual bool PostRequestToQueue(SenderPtr sender, DispatchItemPtr dispatchItem) override;

			virtual void DispatchDSCMessage(SenderPtr sender, DispatchItemPtr dispatchItem) override;
		
			void Send(std::shared_ptr<Session> sender, std::shared_ptr<Packet::RPCPacket> packet);

			inline bool Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size);
		public:
			/// ini all interface 
			virtual void OnDeserialized(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;
			virtual void OnDeserializedFailed(std::string& serverID, const std::shared_ptr<Session> session) override;

			virtual void OnDispatched(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;
			virtual void OnDispatchFailed(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;

			virtual void OnCommited(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;
			virtual void OnServiceNotFound(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;
			virtual void OnServiceParameterInvalid(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;
			virtual void OnServiceError(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;
			virtual void OnServiceEmptyRequest(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;

			virtual void OnCompleted(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;
			virtual void OnFailed(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> requestD) override;
		};
	}
}