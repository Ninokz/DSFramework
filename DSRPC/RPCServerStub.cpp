#include "RPCServerStub.h"

namespace DSFramework {
	namespace DSRPC {
		RPCServerStub::RPCServerStub(std::shared_ptr<RPCEventHandler> rpcEventHandler) : m_rpcEventHandler(rpcEventHandler)
		{

		}

		RPCServerStub::~RPCServerStub()
		{
		}

		void RPCServerStub::OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg)
		{
			LOG_DEBUG_CONSOLE("Stub Layer Data received: " + msg->PrintStringFormat());
			bool deserializeResult = false;
			/// 将接收到的数据反序列化为RPCPacket
			std::shared_ptr<Packet::RPCPacket> packet = this->Deserialize(msg->m_data, msg->m_dataSize, &deserializeResult);
			if (deserializeResult)
			{
				/// 若成功反序列化则执行以下代码
				LOG_DEBUG_CONSOLE("Packet deserialized:\n" + packet->DebugString());
				if (packet->service().empty()) {
					/// 空service则返回错误响应
					packet.reset();
					packet = RPCPacketFactory::CreateErrorResponse(m_serverid, sender->GetUUID(), Packet::RPCPacketError::PKT_EMPTY_REQUEST, sender->GetUUID());
					Send(sender, packet);
					return;
				}

				/// 1. 初始化包并生成请求ID
				std::string requestid = RPCPacketFactory::InitRPCPacket(packet,sender->GetUUID());
				/// 2. 将请求ID和RPCPacket交给EventHandler处理: 目前只有RPCPacketManager实现了IDeserializedEventHandler, 会将请求ID和RPCPacket保存到m_requests中, 处于OnDeserialized事件调用链第一位
				m_rpcEventHandler->OnDeserialized(requestid, sender->GetUUID(), packet);
				/// 3. dispatcher分发请求 todo 完成dispatcher的实现
				/// todo 完成dispatcher的实现
				if (false)
				{
					/// 若成功分发请求则执行以下代码
					LOG_DEBUG_CONSOLE("Request dispatched success");
					/// 4. 通知EventHandler请求已经分发: 目前只有RPCPacketManager实现了IDispatchEventHandler, 会将请求ID的RPCPacket的状态设置为COMMITED, 处于OnDispatched事件调用链第一位
					m_rpcEventHandler->OnDispatched(requestid);
					/// 5. 使用 RPCPacketFactory 生成对应响应包, 深拷贝
					std::shared_ptr<Packet::RPCPacket> response = RPCPacketFactory::CreateResponse(packet);
					/// 5. 设置响应类型
					RPCPacketFactory::ChangeTypeToResponse(packet);
					/// 6. 设置错误码
					RPCPacketFactory::SetErrorCode(response, Packet::RPCPacketError::PKT_NO_ERROR);
					/// 7. 转换收发方(因为该包没有保存至manager)
					RPCPacketFactory::ChangeRecvSend(response);
					/// 8. 发送响应包
					this->Send(sender, response);
				}
				else
				{
					/// 若分发请求失败则执行以下代码
					LOG_DEBUG_CONSOLE("Request dispatched failed");
					/// 4. 通知EventHandler请求分发失败: 目前只有RPCPacketManager实现了IDispatchEventHandler, 会将请求ID的RPCPacket的状态设置为COMPLETED,并且移除这个包 处于OnDispatchFailed事件调用链第一位
					m_rpcEventHandler->OnDispatchFailed(requestid);
					/// 5. 设置响应类型
					RPCPacketFactory::ChangeTypeToResponse(packet);
					/// 6. 设置错误码
					RPCPacketFactory::SetErrorCode(packet, Packet::RPCPacketError::SERVICE_BUSY);
					/// 7. 转换收发方(因为该包没有保存至manager)
					RPCPacketFactory::ChangeRecvSend(packet);
					/// 8. 发送响应包
					this->Send(sender, packet);
				}
			}
			else
			{
				packet.reset();
				packet = RPCPacketFactory::CreateErrorResponse(m_serverid, sender->GetUUID(), Packet::RPCPacketError::PKT_DESERIALIZATION_ERROR, sender->GetUUID());
				Send(sender, packet);
			}
		}

		void RPCServerStub::Send(std::shared_ptr<Session> sender, std::shared_ptr<Packet::RPCPacket> packet)
		{
			const char* data = nullptr;
			size_t size = 0;
			if (this->Serialize(packet, &data, &size))
			{
				if (size > INT32_MAX) {
					LOG_ERROR_CONSOLE("Data size is too large");
					delete[] data;
					data = nullptr;
					return;
				}
				else
				{
					sender->Send(data, (int)size);
					delete[] data;
					data = nullptr;
				}	
			}
			else
			{
				LOG_ERROR_CONSOLE("Failed to serialize error response packet");
			}
		}

		inline std::shared_ptr<RPCPacket> RPCServerStub::Deserialize(const char* data, int datalength, bool* serializeResult)
		{
			std::shared_ptr<RPCPacket> packet = std::make_shared<RPCPacket>();
			*serializeResult = packet->ParseFromArray(data, datalength);
			return packet;
		}

		inline bool RPCServerStub::Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size)
		{
			if (!packet || !data || !size) {
				return false;
			}

			*size = packet->ByteSizeLong();

			*data = new char[*size];
			if (!*data) {
				return false;
			}

			if (*size > INT32_MAX)
			{
				delete[] * data;
				*data = nullptr;
				*size = 0;
				return false;
			}

			if (!packet->SerializeToArray(const_cast<char*>(*data), (int)*size)) {
				delete[] * data;
				*data = nullptr;
				*size = 0;
				return false;
			}
			return true;
		}
	}
}