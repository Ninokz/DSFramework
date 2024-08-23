#include "ResponseDispatcher.h"

namespace DSFramework {
	namespace DSRPC {
		ResponseDispatcher::ResponseDispatcher(size_t maxWaitedDispatch) : Dispatcher(maxWaitedDispatch)
		{
			Start();
			LOG_INFO_CONSOLE("ResponseDispatcher Started");
		}

		ResponseDispatcher::~ResponseDispatcher()
		{
			LOG_INFO_CONSOLE("ResponseDispatcher will destory");
		}

		bool ResponseDispatcher::PostRequestToQueue(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			if (this->m_requestQueue->size() < this->m_maxWaitedDispatch)
			{
				this->m_requestQueue->Push(std::make_pair(sender, dispatchItem));
				LOG_DEBUG_CONSOLE("Response dispatched success");
				return true;
			}
			else
			{
				LOG_DEBUG_CONSOLE("Response dispatched failed");
				return false;
			}
		}

		void ResponseDispatcher::DispatchDSCMessage(SenderPtr sender, DispatchItemPtr dispatchItem)
		{
			Send(sender, dispatchItem);
		}

		void ResponseDispatcher::Send(std::shared_ptr<Session> sender, std::shared_ptr<Packet::RPCPacket> packet)
		{
			if (!packet)
			{
				LOG_ERROR_CONSOLE("Empty packet to send");
				return;
			}
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
					sender->Send(data, static_cast<int>(size));
					delete[] data;
					data = nullptr;
				}
			}
			else
			{
				LOG_ERROR_CONSOLE("Failed to serialize error response packet");
			}
		}

		inline bool ResponseDispatcher::Serialize(const std::shared_ptr<RPCPacket> packet, const char** data, size_t* size)
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
		
		void ResponseDispatcher::OnDeserialized(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			auto response = RPCPacketFactory::CreatePacketResponse(request);
			this->PostRequestToQueue(session, response);
		}
		
		void ResponseDispatcher::OnDeserializedFailed(std::string& senderID, const std::shared_ptr<Session> session)
		{
			std::string to = session->GetUUID();
			auto response = RPCPacketFactory::CreateDeserializedErrorPacket(senderID, to, to);
			this->PostRequestToQueue(session, response);
		}
		
		void ResponseDispatcher::OnDispatched(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			auto response = RPCPacketFactory::CreatePacketResponse(request);
			this->PostRequestToQueue(session, response);
		}
		
		void ResponseDispatcher::OnDispatchFailed(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			auto response = RPCPacketFactory::CreateDispatchedFailedResponsePacket(request);
			this->PostRequestToQueue(session, response);
		}
		
		void ResponseDispatcher::OnCommited(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			auto response = RPCPacketFactory::CreatePacketResponse(request);
			this->PostRequestToQueue(session, response);
		}
		
		void ResponseDispatcher::OnServiceNotFound(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			auto response = RPCPacketFactory::CreateServiceNotFoundResponsePacket(request);
			this->PostRequestToQueue(session, response);
		}
		
		void ResponseDispatcher::OnServiceParameterInvalid(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			auto response = RPCPacketFactory::CreateServiceParameterInvalidResponsePacket(request);
			this->PostRequestToQueue(session, response);
		}
		
		void ResponseDispatcher::OnServiceError(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			auto response = RPCPacketFactory::CreateServiceErrorResponsePacket(request);
			this->PostRequestToQueue(session, response);
		}

		void ResponseDispatcher::OnServiceEmptyRequest(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			auto response = RPCPacketFactory::CreateEmptyRequestErrorPacket(request);
			this->PostRequestToQueue(session, response);
		}
		
		void ResponseDispatcher::OnCompleted(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> request)
		{
			auto response = RPCPacketFactory::CreatePacketResponse(request);
			this->PostRequestToQueue(session, response);
		}
		
		void ResponseDispatcher::OnFailed(const std::shared_ptr<Session> session, std::shared_ptr<RPCPacket> requestD)
		{
			auto response = RPCPacketFactory::CreateServiceErrorResponsePacket(requestD);
			this->PostRequestToQueue(session, response);
		}
	}
}