#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <shared_mutex>
#include <mutex>

#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "RPCPacket.pb.h"

using DSFramework::DSRPC::Packet::RPCPacket;
namespace DSFramework {
	namespace DSRPC {
		class RPCPacketManager
		{
		private:
			std::unordered_map<std::string, std::shared_ptr<Packet::RPCPacket>> m_requests;
			std::shared_mutex m_requestsMutex;
		public:
			using REQUEST_ID = std::string;

			RPCPacketManager();
			virtual ~RPCPacketManager();

			REQUEST_ID AddRequest(std::shared_ptr<Packet::RPCPacket> request);
			void RemoveRequest(REQUEST_ID& requestID);
			void UpdateRequestStatus(REQUEST_ID& requestID, Packet::RPCPacketStatus status);
			std::shared_ptr<Packet::RPCPacket> QueryRequest(REQUEST_ID& requestID, bool* queryResult);
		};
	}
}