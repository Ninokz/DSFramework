#pragma once
#include <string>
#include <memory>

#include "../DSCommunication/Session.h"

#include "RPCProcessor.h"
#include "Dispatcher.h"
#include "RPCPacket.pb.h"
#include "RPCEventHandler.h"

using DSFramework::DSCommunication::Session;
using DSFramework::DSRPC::Packet::RPCPacket;
using DSFramework::DSRPC::RPCEventHandler;

namespace DSFramework {
	namespace DSRPC {
		class RequestDispatcher : public Dispatcher<Session, RPCPacket>, public IDeserializedEventHandler
		{
		private:
			RPCEventHandler& m_rpcEventHandler;
		public:
			RequestDispatcher(size_t maxWaitedDispatch, RPCEventHandler& m_rpcEventHandler);
			virtual ~RequestDispatcher();
		private:
			virtual bool PostRequestToQueue(SenderPtr sender, DispatchItemPtr dispatchItem) override;

			virtual void DispatchDSCMessage(SenderPtr sender, DispatchItemPtr dispatchItem) override;
		public:
			virtual void OnDeserialized(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request) override;
		};
	}
}