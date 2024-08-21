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
			static std::shared_ptr<RPCPacket> CreateErrorResponse(std::string from, std::string to, Packet::RPCPacketStatus status, Packet::RPCPacketError error, std::string innerID) {
				{
					std::shared_ptr<RPCPacket> response = std::make_shared<RPCPacket>();
					response->set_type(Packet::DEFAULT_RESPONSE);
					response->set_error(error);
					response->set_status(status);
					response->set_inner_id(innerID);

					response->set_from(from);
					response->set_to(to);
					return response;
				}
			}

			static std::shared_ptr<RPCPacket> CreateResponseFromOrign(std::shared_ptr<RPCPacket> orign, Packet::RPCPacketStatus status, Packet::RPCPacketError error, std::string innerID) {
				///deep copy from orign
				std::shared_ptr<RPCPacket> response = std::make_shared<RPCPacket>();
				response->CopyFrom(*orign);

				if ((orign->type() & Packet::RPCPacketType::TASK_REQUEST) == Packet::RPCPacketType::TASK_REQUEST)
					response->set_type(Packet::RPCPacketType::TASK_RESPONSE);
				else if ((orign->type() & Packet::RPCPacketType::QUERY_REQUEST) == Packet::RPCPacketType::QUERY_REQUEST)
					response->set_type(Packet::RPCPacketType::QUERY_RESPONSE);

				response->set_error(error);
				response->set_status(status);
				response->set_inner_id(innerID);

				std::string or_from = orign->from();
				std::string or_to = orign->to();
				response->set_from(or_to);
				response->set_to(or_from);

				return response;
			}

			static void UpdateRPCPacketStatus(std::shared_ptr<RPCPacket> packet, Packet::RPCPacketStatus status) {
				if ((status & Packet::RPCPacketStatus::COMMITED) == Packet::RPCPacketStatus::COMMITED)
				{
					packet->set_status(Packet::RPCPacketStatus::COMMITED);
					packet->set_commited_time(CurrentTime());
				}
				else if ((status & Packet::RPCPacketStatus::COMPLETED) == Packet::RPCPacketStatus::COMPLETED)
				{
					packet->set_status(Packet::RPCPacketStatus::COMPLETED);
					packet->set_completed_time(CurrentTime());
				}
				else if ((status & Packet::RPCPacketStatus::WAITING) == Packet::RPCPacketStatus::WAITING)
				{
					packet->set_status(Packet::RPCPacketStatus::WAITING);
					packet->set_created_time(CurrentTime());
				}
			}
		};
	}
}
