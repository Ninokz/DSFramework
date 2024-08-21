#pragma once
#include <memory>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "RPCPacket.pb.h"

using DSFramework::DSRPC::Packet::RPCPacket;
namespace DSFramework {
	namespace DSRPC {
		namespace Packet {
			class RPCPacketFactory
			{
			private:
				static inline std::string CurrentTime() {
					return boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time());
				}
			public:
				static std::shared_ptr<RPCPacket> CreateErrorResponse(std::string from, std::string to, Packet::RPCPacketStatus status, Packet::RPCPacketError error, std::string innerID)
				{
					std::shared_ptr<RPCPacket> response = std::make_shared<RPCPacket>();
					response->clear_type();
					response->set_error(error);
					response->set_status(status);
					response->set_inner_id(innerID);

					response->set_from(from);
					response->set_to(to);

					response->clear_created_time();
					response->clear_commited_time();
					response->clear_completed_time();
					response->clear_service();
					response->clear_request_id();
					response->clear_parameters();
					response->clear_result();
					return response;
				}

				static std::shared_ptr<RPCPacket> CreateResponseFromOrign(std::shared_ptr<RPCPacket> orign, Packet::RPCPacketStatus status, Packet::RPCPacketError error,
					std::string innerID)
				{
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
						packet->set_commited_time("");
						packet->set_completed_time("");
					}
				}

				static std::shared_ptr<RPCPacket> Deserialize(const char* data, size_t datalength, bool* serializeResult)
				{
					std::shared_ptr<RPCPacket> packet = std::make_shared<RPCPacket>();
					*serializeResult = packet->ParseFromArray(data, datalength);
					return packet;
				}

				static bool Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size)
				{
					if (!packet) {
						return false;
					}

					std::string serialized_data;
					if (!packet->SerializeToString(&serialized_data)) {
						return false;
					}

					char* buffer = new char[serialized_data.size()];
					memcpy(buffer, serialized_data.data(), serialized_data.size());

					*data = buffer;
					*size = serialized_data.size();
					return true;
				}				
			};
		}
	}
}
