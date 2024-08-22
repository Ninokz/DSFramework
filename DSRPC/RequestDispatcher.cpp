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
			/// ��������
			/// ������
			/// ������m_serviceProvider����
		}

		void RequestDispatcher::HandlePostSuccess(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			/// ���ɹ��ַ�������ִ�����´���
			LOG_DEBUG_CONSOLE("Request dispatched success");
			/// 4. ֪ͨEventHandler�����Ѿ��ַ�: Ŀǰֻ��RPCPacketManagerʵ����IDispatchEventHandler, �Ὣ����ID��RPCPacket��״̬����ΪCOMMITED, ����OnDispatched�¼���������һλ
			m_rpcEventHandler.OnDispatched(dispatchItem->request_id());
			/// 5. ʹ�� RPCPacketFactory ���ɶ�Ӧ��Ӧ��, ���
			std::shared_ptr<Packet::RPCPacket> response = RPCPacketFactory::CreateCopy(dispatchItem);
			/// 5. ������Ӧ����
			RPCPacketFactory::ChangeTypeToResponse(response);
			/// 6. ���ô�����
			RPCPacketFactory::SetErrorCode(response, Packet::RPCPacketError::PKT_NO_ERROR);
			/// 7. ת���շ���(��Ϊ�ð�û�б�����manager)
			RPCPacketFactory::ChangeRecvSend(response);
			/// 8. ������Ӧ��
			this->Send(sender, response);
		}

		void RequestDispatcher::HandlePostFaile(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			/// ���ַ�����ʧ����ִ�����´���
			LOG_DEBUG_CONSOLE("Request dispatched failed");
			/// 4. ֪ͨEventHandler����ַ�ʧ��: Ŀǰֻ��RPCPacketManagerʵ����IDispatchEventHandler, �Ὣ����ID��RPCPacket��״̬����ΪCOMPLETED,�����Ƴ������ ����OnDispatchFailed�¼���������һλ
			m_rpcEventHandler.OnDispatchFailed(dispatchItem->request_id());
			/// 5. ������Ӧ����
			RPCPacketFactory::ChangeTypeToResponse(dispatchItem);
			/// 6. ���ô�����
			RPCPacketFactory::SetErrorCode(dispatchItem, Packet::RPCPacketError::SERVICE_BUSY);
			/// 7. ת���շ���(��Ϊ�ð�û�б�����manager)
			RPCPacketFactory::ChangeRecvSend(dispatchItem);
			/// 8. ������Ӧ��
			this->Send(sender, dispatchItem);
		}

		void RequestDispatcher::HandleServiceNotFound(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			LOG_DEBUG_CONSOLE("Request Service Not found");
			m_rpcEventHandler.OnServiceNotFound(dispatchItem->request_id());
			/// 5. ������Ӧ����
			RPCPacketFactory::ChangeTypeToResponse(dispatchItem);
			/// 6. ���ô�����
			RPCPacketFactory::SetErrorCode(dispatchItem, Packet::RPCPacketError::SERVICE_NOT_FOUND);
			/// 7. ת���շ���(��Ϊ�ð�û�б�����manager)
			RPCPacketFactory::ChangeRecvSend(dispatchItem);
			/// 8. ������Ӧ��
			this->Send(sender, dispatchItem);
		}

		void RequestDispatcher::HandleServiceParameterInvalid(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			LOG_DEBUG_CONSOLE("Request Service Parameter invalid");
			m_rpcEventHandler.OnServiceParameterInvalid(dispatchItem->request_id());
			/// 5. ������Ӧ����
			RPCPacketFactory::ChangeTypeToResponse(dispatchItem);
			/// 6. ���ô�����
			RPCPacketFactory::SetErrorCode(dispatchItem, Packet::RPCPacketError::SERVICE_IVAILD_PARAMETERS);
			/// 7. ת���շ���(��Ϊ�ð�û�б�����manager)
			RPCPacketFactory::ChangeRecvSend(dispatchItem);
			/// 8. ������Ӧ��
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