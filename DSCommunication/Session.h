#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <queue>
#include <mutex>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "DSCPacket.h"
#include "Logger.h"

using boost::uuids::uuid;
using boost::asio::ip::tcp;
using DSFramework::DSComponent::Logger;
using DSFramework::DSComponent::Log;

namespace DSFramework {
	namespace DSCommunication {
		const uint8_t SEND_QUEUE_MAX_SIZE = 32;

		class EventHandler;
		class Session : public std::enable_shared_from_this<Session>
		{
		private:
			std::string m_uuid;
			tcp::socket m_socket;

			std::shared_ptr<EventHandler> m_eventHandlerPtr;
	
			std::mutex m_sendQueueMutex;
			std::uint8_t m_sendQueueMaxSize;
			std::queue<std::shared_ptr<DSCSendPacket>> m_sendQueue;

			std::shared_ptr<DSCRecvPacket> m_cached_HeadRecvPacket;
			std::shared_ptr<DSCRecvPacket> m_cached_BodyRecvPacket;

			bool m_closed = true;
		public:
			tcp::socket& GetSocket() { return m_socket; }
			std::string GetUUID() { return m_uuid; }
		public:
			Session(boost::asio::io_context& ioContext, std::shared_ptr<EventHandler> eventHandler, uint8_t sendQMaxSize);
			virtual ~Session();

			void Start();
			void Close();

			void Send(const char* data, int dataLength);
		private:
			void HandleHeadRead(const boost::system::error_code& ec, size_t bytes_tramsferred);
			void HandleBodyRead(const boost::system::error_code& ec, size_t bytes_tramsferred);
			void HandleWrite(const boost::system::error_code& ec);
		};
	}
}
