#pragma once
#include <string>
#include <memory>
#include <atomic>
#include <vector>

#include "../DSCommunication/ThreadPool.h"

#include "RPCPacket.pb.h"
#include "RPCEventHandler.h"


using DSFramework::DSRPC::Packet::RPCPacket;
using DSFramework::DSRPC::RPCEventHandler;
namespace DSFramework {
	namespace DSRPC {

		class RPCProcedure : public std::enable_shared_from_this<RPCProcedure>
		{
		public:
			std::string m_procedureId;
			std::atomic<bool> m_isRunning;
			std::string m_requestID;
			std::string m_sessionID;
			std::shared_ptr<RPCPacket> m_request;
		private:
			std::shared_ptr<RPCEventHandler> m_rpcEventHandler;
		public:
			RPCProcedure(std::shared_ptr<RPCPacket> request, std::shared_ptr<RPCEventHandler> rpcEventHandler):
				m_isRunning(false),
				m_requestID(request->request_id()),
				m_sessionID(request->inner_id()),
				m_request(request),
				m_rpcEventHandler(rpcEventHandler){}
			virtual ~RPCProcedure() = default;
			virtual void Execute(std::shared_ptr<RPCPacket> request) = 0;
		};
	}
}