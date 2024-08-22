#include "RequestDispatcher.h"


namespace DSFramework {
	namespace DSRPC {
		RequestDispatcher::RequestDispatcher(size_t maxWaitedDispatch, RPCEventHandler& rpcEventHandler) :
			Dispatcher(maxWaitedDispatch),
			m_rpcEventHandler(rpcEventHandler)
		{
		}

		RequestDispatcher::~RequestDispatcher()
		{
		}

		bool RequestDispatcher::PostRequestToQueue(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			if (this->m_requestQueue->size() < this->m_maxWaitedDispatch)
			{
				this->m_requestQueue->Push(std::make_pair(sender, dispatchItem));
				HandlePostSuccess(sender, dispatchItem);
				return true;
			}
			else
			{
				HandlePostFaile(sender, dispatchItem);
				return false;
			}
		}

		void RequestDispatcher::DispatchDSCMessage(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			/// 搜索服务
			/// 检查参数
			/// 交付给m_serviceProvider处理
		}

		void RequestDispatcher::HandlePostSuccess(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			/// 若成功分发请求则执行以下代码
			LOG_DEBUG_CONSOLE("Request dispatched success");
			/// 4. 通知EventHandler请求已经分发: 目前只有RPCPacketManager实现了IDispatchEventHandler, 会将请求ID的RPCPacket的状态设置为COMMITED, 处于OnDispatched事件调用链第一位
			m_rpcEventHandler.OnDispatched(dispatchItem->request_id());
			/// 5. 使用 RPCPacketFactory 生成对应响应包, 深拷贝
			std::shared_ptr<Packet::RPCPacket> response = RPCPacketFactory::CreateCopy(dispatchItem);
			/// 5. 设置响应类型
			RPCPacketFactory::ChangeTypeToResponse(response);
			/// 6. 设置错误码
			RPCPacketFactory::SetErrorCode(response, Packet::RPCPacketError::PKT_NO_ERROR);
			/// 7. 转换收发方(因为该包没有保存至manager)
			RPCPacketFactory::ChangeRecvSend(response);
			/// 8. 发送响应包
			this->Send(sender, response);
		}

		void RequestDispatcher::HandlePostFaile(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			/// 若分发请求失败则执行以下代码
			LOG_DEBUG_CONSOLE("Request dispatched failed");
			/// 4. 通知EventHandler请求分发失败: 目前只有RPCPacketManager实现了IDispatchEventHandler, 会将请求ID的RPCPacket的状态设置为COMPLETED,并且移除这个包 处于OnDispatchFailed事件调用链第一位
			m_rpcEventHandler.OnDispatchFailed(dispatchItem->request_id());
			/// 5. 设置响应类型
			RPCPacketFactory::ChangeTypeToResponse(dispatchItem);
			/// 6. 设置错误码
			RPCPacketFactory::SetErrorCode(dispatchItem, Packet::RPCPacketError::SERVICE_BUSY);
			/// 7. 转换收发方(因为该包没有保存至manager)
			RPCPacketFactory::ChangeRecvSend(dispatchItem);
			/// 8. 发送响应包
			this->Send(sender, dispatchItem);
		}

		void RequestDispatcher::HandleServiceNotFound(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			LOG_DEBUG_CONSOLE("Request Service Not found");
			m_rpcEventHandler.OnServiceNotFound(dispatchItem->request_id());
			/// 5. 设置响应类型
			RPCPacketFactory::ChangeTypeToResponse(dispatchItem);
			/// 6. 设置错误码
			RPCPacketFactory::SetErrorCode(dispatchItem, Packet::RPCPacketError::SERVICE_NOT_FOUND);
			/// 7. 转换收发方(因为该包没有保存至manager)
			RPCPacketFactory::ChangeRecvSend(dispatchItem);
			/// 8. 发送响应包
			this->Send(sender, dispatchItem);
		}

		void RequestDispatcher::HandleServiceParameterInvalid(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			LOG_DEBUG_CONSOLE("Request Service Parameter invalid");
			m_rpcEventHandler.OnServiceParameterInvalid(dispatchItem->request_id());
			/// 5. 设置响应类型
			RPCPacketFactory::ChangeTypeToResponse(dispatchItem);
			/// 6. 设置错误码
			RPCPacketFactory::SetErrorCode(dispatchItem, Packet::RPCPacketError::SERVICE_IVAILD_PARAMETERS);
			/// 7. 转换收发方(因为该包没有保存至manager)
			RPCPacketFactory::ChangeRecvSend(dispatchItem);
			/// 8. 发送响应包
			this->Send(sender, dispatchItem);
		}

		void RequestDispatcher::Send(std::shared_ptr<Session> sender, std::shared_ptr<Packet::RPCPacket> packet)
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

		inline bool RequestDispatcher::Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size)
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