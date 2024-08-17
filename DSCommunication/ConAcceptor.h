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

#include "Session.h"
#include "SessionManager.h"
#include "Logger.h"

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
			IEventHandler& m_eventHandler;
		public:
			ConAcceptor(boost::asio::io_context& ioc, short port, IEventHandler& eventHandler);
			virtual ~ConAcceptor();
		private:
			void StartAccept();
			void HandleAccept(std::shared_ptr<Session> new_session, const boost::system::error_code& ec);
		};
	}
}
