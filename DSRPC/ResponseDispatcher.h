#pragma once
#include <string>
#include <memory>

#include "../DSCommunication/Session.h"

#include "RPCServer.h"
#include "Dispatcher.h"
#include "RPCPacket.pb.h"
#include "RPCPacketFactory.h"


using DSFramework::DSCommunication::Session;
using DSFramework::DSRPC::Packet::RPCPacket;
namespace DSFramework {
	namespace DSRPC {
		class ResponseDispatcher : public Dispatcher<Session, RPCPacket>
		{
		public:
			ResponseDispatcher(size_t maxWaitedDispatch);
			virtual ~ResponseDispatcher();

			virtual bool PostRequestToQueue(SenderPtr sender, DispatchItemPtr dispatchItem) override;

			virtual void DispatchDSCMessage(SenderPtr sender, DispatchItemPtr dispatchItem) override;
		private:
			void Send(std::shared_ptr<Session> sender, std::shared_ptr<Packet::RPCPacket> packet);

			inline bool Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size);
		};
	}
}