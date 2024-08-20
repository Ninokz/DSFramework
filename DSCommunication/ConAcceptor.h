#pragma once
#ifdef _MSC_VER
#define _WIN32_WINNT 0x0601
#endif

#include <mutex>
#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include "Logger.h"
#include "Session.h"
#include "SessionManager.h"


using std::unordered_map;
using boost::asio::ip::tcp;
using boost::asio::io_context;
using DSFramework::DSComponent::Logger;
using DSFramework::DSComponent::Log;

namespace DSFramework {
	namespace DSCommunication {
		class ConAcceptor
		{
		private:
			tcp::acceptor m_acceptor;
			boost::asio::io_context& m_ioc;
			std::shared_ptr<EventHandler> m_eventHandlerPtr;
		public:
			ConAcceptor(boost::asio::io_context& ioc, short port, std::shared_ptr<EventHandler> eventHandler);
			virtual ~ConAcceptor();
		private:
			void StartAccept();
			void HandleAccept(std::shared_ptr<Session> new_session, const boost::system::error_code& ec);
		};
	}
}
