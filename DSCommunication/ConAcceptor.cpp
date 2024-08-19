#include "ConAcceptor.h"
#include "Session.h"


using DSFramework::DSComponent::IOServicePool;
namespace DSFramework {
	namespace DSCommunication {
		ConAcceptor::ConAcceptor(boost::asio::io_context& ioc, short port, EventHandler& eventHandler) :
			m_ioc(ioc), m_acceptor(ioc, tcp::endpoint(tcp::v4(), port)), m_eventHandler(eventHandler)
		{
			StartAccept();
			LOG_INFO_CONSOLE("Connection acceptor started");
		}

		ConAcceptor::~ConAcceptor()
		{
			this->m_acceptor.close();
			LOG_INFO_CONSOLE("Connection acceptor closed");
		}

		void ConAcceptor::StartAccept() {
			auto& ioc = IOServicePool::GetInstance()->getIOContext();
			std::shared_ptr<Session> new_session = std::make_shared<Session>(ioc, &(this->m_eventHandler), SEND_QUEUE_MAX_SIZE);			
			m_acceptor.async_accept(new_session->GetSocket(), 
				boost::bind(&ConAcceptor::HandleAccept, 
					this, new_session, boost::asio::placeholders::error));
		}

		void ConAcceptor::HandleAccept(std::shared_ptr<Session> new_session, const boost::system::error_code& ec) {
			if (!ec) {
				new_session->Start();
			}
			else {
				new_session->Close();
				LOG_WARN_CONSOLE("Error while accepting connection: " + std::to_string(ec.value()) + " " + ec.message());
			}
			StartAccept();
		}
	}
}