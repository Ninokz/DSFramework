#pragma once
#include <memory>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "RPCPacket.pb.h"

using DSFramework::DSRPC::Packet::RPCPacket;
namespace DSFramework {
	namespace DSRPC {
		class RPCPacketFactory
		{
		private:
			static inline std::string CurrentTime() { return boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time()); }

			static inline std::shared_ptr<RPCPacket> CreateCopy(std::shared_ptr<RPCPacket> request)
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
		public:
			static std::shared_ptr<RPCPacket> CreateDeserializedErrorPacket(std::string from, std::string to, std::string innerID)
			{
				std::shared_ptr<RPCPacket> response = std::make_shared<RPCPacket>();
				response->set_type(Packet::DEFAULT_RESPONSE);
				response->set_error(Packet::RPCPacketError::PKT_DESERIALIZATION_ERROR);
				response->set_status(Packet::RPCPacketStatus::FAILED);
				response->set_inner_id(innerID);

				response->set_from(from);
				response->set_to(to);
				return response;
			}

			static std::shared_ptr<RPCPacket> CreatePacketResponse(std::shared_ptr<RPCPacket> request)
			{
				/// 5. 使用 RPCPacketFactory 生成对应响应包, 深拷贝
				std::shared_ptr<RPCPacket> response = RPCPacketFactory::CreateCopy(request);
				/// 5. 设置响应类型
				RPCPacketFactory::ChangeTypeToResponse(response);
				/// 6. 设置错误码
				RPCPacketFactory::SetErrorCode(response, Packet::RPCPacketError::PKT_NO_ERROR);
				/// 7. 转换收发方(因为该包没有保存至manager)
				RPCPacketFactory::ChangeRecvSend(response);
				return response;
			}

			static std::shared_ptr<RPCPacket> CreateEmptyRequestErrorPacket(const std::shared_ptr<RPCPacket> orign)
			{
				/// 5. 使用 RPCPacketFactory 生成对应响应包, 深拷贝
				std::shared_ptr<Packet::RPCPacket> response = RPCPacketFactory::CreateCopy(orign);
				/// 5. 设置响应类型
				RPCPacketFactory::ChangeTypeToResponse(response);
				/// 6. 设置错误码
				RPCPacketFactory::SetErrorCode(response, Packet::RPCPacketError::PKT_EMPTY_REQUEST);
				/// 7. 转换收发方(因为该包没有保存至manager)
				RPCPacketFactory::ChangeRecvSend(response);
				return response;
			}

			static std::shared_ptr<RPCPacket> CreateDispatchedFailedResponsePacket(const std::shared_ptr<RPCPacket> orign)
			{
				/// 5. 使用 RPCPacketFactory 生成对应响应包, 深拷贝
				std::shared_ptr<Packet::RPCPacket> response = RPCPacketFactory::CreateCopy(orign);
				/// 5. 设置响应类型
				RPCPacketFactory::ChangeTypeToResponse(response);
				/// 6. 设置错误码
				RPCPacketFactory::SetErrorCode(response, Packet::RPCPacketError::SERVICE_BUSY);
				/// 7. 转换收发方(因为该包没有保存至manager)
				RPCPacketFactory::ChangeRecvSend(response);
				return response;
			}

			static std::shared_ptr<RPCPacket> CreateServiceNotFoundResponsePacket(const std::shared_ptr<RPCPacket> orign)
			{
				/// 5. 使用 RPCPacketFactory 生成对应响应包, 深拷贝
				std::shared_ptr<Packet::RPCPacket> response = RPCPacketFactory::CreateCopy(orign);
				/// 5. 设置响应类型
				RPCPacketFactory::ChangeTypeToResponse(response);
				/// 6. 设置错误码
				RPCPacketFactory::SetErrorCode(response, Packet::RPCPacketError::SERVICE_NOT_FOUND);
				/// 7. 转换收发方(因为该包没有保存至manager)
				RPCPacketFactory::ChangeRecvSend(response);
				return response;			
			}

			static std::shared_ptr<RPCPacket> CreateServiceParameterInvalidResponsePacket(const std::shared_ptr<RPCPacket> orign)
			{
				/// 5. 使用 RPCPacketFactory 生成对应响应包, 深拷贝
				std::shared_ptr<Packet::RPCPacket> response = RPCPacketFactory::CreateCopy(orign);
				/// 5. 设置响应类型
				RPCPacketFactory::ChangeTypeToResponse(response);
				/// 6. 设置错误码
				RPCPacketFactory::SetErrorCode(response, Packet::RPCPacketError::SERVICE_IVAILD_PARAMETERS);
				/// 7. 转换收发方(因为该包没有保存至manager)
				RPCPacketFactory::ChangeRecvSend(response);
				return response;
			}

			static std::shared_ptr<RPCPacket> CreateServiceErrorResponsePacket(const std::shared_ptr<RPCPacket> orign)
			{
				/// 5. 使用 RPCPacketFactory 生成对应响应包, 深拷贝
				std::shared_ptr<Packet::RPCPacket> response = RPCPacketFactory::CreateCopy(orign);
				/// 5. 设置响应类型
				RPCPacketFactory::ChangeTypeToResponse(response);
				/// 6. 设置错误码
				RPCPacketFactory::SetErrorCode(response, Packet::RPCPacketError::SERVICE_ERROR);
				/// 7. 转换收发方(因为该包没有保存至manager)
				RPCPacketFactory::ChangeRecvSend(response);
				return response;
			}
		};
	}
}
