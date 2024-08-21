#pragma once
#include <memory>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "RPCPacket.pb.h"

using DSFramework::DSRPC::Packet::RPCPacket;
namespace DSFramework {
	namespace DSRPC {
		class RPCPacketFactory
		{
		private:
			static inline std::string CurrentTime() { return boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time()); }
		public:
			static std::shared_ptr<RPCPacket> CreateErrorResponse(std::string from, std::string to, Packet::RPCPacketError error, std::string innerID) {
				{
					std::shared_ptr<RPCPacket> response = std::make_shared<RPCPacket>();
					response->set_type(Packet::DEFAULT_RESPONSE);
					response->set_error(error);
					response->set_status(Packet::RPCPacketStatus::WAITING);
					response->set_inner_id(innerID);

					response->set_from(from);
					response->set_to(to);
					return response;
				}
			}

			static inline std::shared_ptr<RPCPacket> CreateResponse(std::shared_ptr<RPCPacket> request)
			{
				/// deep copy request to response
				std::shared_ptr<RPCPacket> response = std::make_shared<RPCPacket>();
				response->CopyFrom(*request);
				return response;
			}

			static inline void ChangeTypeToResponse(std::shared_ptr<RPCPacket> packet)
			{
				if((packet->type() & Packet::RPCPacketType::TASK_REQUEST) == Packet::RPCPacketType::TASK_REQUEST)
					packet->set_type(Packet::RPCPacketType::TASK_RESPONSE);
				else if ((packet->type() & Packet::RPCPacketType::TASK_RESPONSE) == Packet::RPCPacketType::TASK_RESPONSE)
					packet->set_type(Packet::RPCPacketType::TASK_REQUEST);
				else
					packet->set_type(Packet::DEFAULT_RESPONSE);
			}

			static inline void SetErrorCode(std::shared_ptr<RPCPacket> packet, Packet::RPCPacketError error)
			{
				packet->set_error(error);
			}

			static inline void ChangeRecvSend(std::shared_ptr<RPCPacket> packet)
			{
				std::string temp = packet->from();
				packet->set_from(packet->to());
				packet->set_to(temp);
			}

			static inline std::string InitRPCPacket(std::shared_ptr<RPCPacket> packet, std::string innerID)
			{
				std::string uid = boost::uuids::to_string(boost::uuids::random_generator()());
				packet->set_request_id(uid);
				packet->set_status(Packet::RPCPacketStatus::WAITING);
				packet->set_error(Packet::RPCPacketError::PKT_NO_ERROR);
				packet->set_inner_id(innerID);
				packet->set_created_time(CurrentTime());
				packet->set_commited_time("");
				packet->set_completed_time("");
				return uid;
			}
		};
	}
}
