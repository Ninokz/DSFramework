#pragma once
#include <string>
#include <memory>

#include "../DSCommunication/Session.h"

#include "RPCServer.h"
#include "Dispatcher.h"
#include "RPCPacket.pb.h"
#include "RPCPacketFactory.h"

using DSFramework::DSCommunication::Session;
using DSFramework::DSRPC::RPCEventHandler;
using DSFramework::DSRPC::RPCPacketFactory;
using DSFramework::DSRPC::Packet::RPCPacket;
namespace DSFramework {
	namespace DSRPC {
		class RequestDispatcher : public Dispatcher<Session, RPCPacket>
		{
		private:
			std::shared_ptr<RPCEventHandler> m_rpcEventHandler;
		public:
			RequestDispatcher(size_t maxWaitedDispatch,std::shared_ptr<RPCEventHandler> rpcEventHandler);
			virtual ~RequestDispatcher();

			virtual bool PostRequestToQueue(SenderPtr sender, DispatchItemPtr dispatchItem) override ;

			virtual void DispatchDSCMessage(SenderPtr sender, DispatchItemPtr dispatchItem) override;
		private:
			void HandlePostSuccess(SenderPtr sender, DispatchItemPtr dispatchItem);

			void HandlePostFaile(SenderPtr sender, DispatchItemPtr dispatchItem);

			void HandleServiceNotFound(SenderPtr sender, DispatchItemPtr dispatchItem);

			void HandleServiceParameterInvalid(SenderPtr sender, DispatchItemPtr dispatchItem);

			void Send(std::shared_ptr<Session> sender, std::shared_ptr<Packet::RPCPacket> packet);

			inline bool Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size);
		};
	}
}