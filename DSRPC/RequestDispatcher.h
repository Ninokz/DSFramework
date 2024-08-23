#pragma once
#include <string>
#include <memory>

#include "../DSCommunication/Session.h"

#include "RPCServer.h"
#include "Dispatcher.h"
#include "RPCPacket.pb.h"
#include "RPCPacketFactory.h"
#include "RPCEventHandler.h"

using DSFramework::DSCommunication::Session;
using DSFramework::DSRPC::Packet::RPCPacket;
using DSFramework::DSRPC::RPCEventHandler;

namespace DSFramework {
	namespace DSRPC {
		class RequestDispatcher : public Dispatcher<Session, RPCPacket> , public IDeserializedEventHandler
		{
		private:
			RPCEventHandler& m_rpcEventHandler;
		public:
			RequestDispatcher(size_t maxWaitedDispatch, RPCEventHandler& rpcEventHandler);
			virtual ~RequestDispatcher();
		private:
			virtual bool PostRequestToQueue(SenderPtr sender, DispatchItemPtr dispatchItem) override ;

			virtual void DispatchDSCMessage(SenderPtr sender, DispatchItemPtr dispatchItem) override;
		
			void HandlePostSuccess(SenderPtr sender, DispatchItemPtr dispatchItem);

			void HandlePostFaile(SenderPtr sender, DispatchItemPtr dispatchItem);

			void HandleCommited(SenderPtr sender, DispatchItemPtr dispatchItem);

			void HandleServiceNotFound(SenderPtr sender, DispatchItemPtr dispatchItem);

			void HandleServiceParameterInvalid(SenderPtr sender, DispatchItemPtr dispatchItem);
		public:
			void OnDeserialized(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;
		};
	}
}