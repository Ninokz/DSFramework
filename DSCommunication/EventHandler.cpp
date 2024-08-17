#include "EventHandler.h"


namespace DSFramework {
	namespace DSCommunication {
		EventHandler::~EventHandler()
		{
			m_closeEventHandlers.clear();
			m_connectEventHandlers.clear();
			m_dataEventHandlers.clear();
		}
		void EventHandler::AddCloseEventHandler(std::shared_ptr<ICloseEventHandler> handler)
		{
			m_closeEventHandlers.push_back(handler);
		}

		void  EventHandler::AddConnectEventHandler(std::shared_ptr<IConnectEventHandler> handler)
		{
			m_connectEventHandlers.push_back(handler);
		}

		void  EventHandler::AddDataEventHandler(std::shared_ptr<IDataEventHandler> handler)
		{
			m_dataEventHandlers.push_back(handler);
		}

		void  EventHandler::OnClose(std::shared_ptr<Session> sender)
		{
			for (auto& handler : m_closeEventHandlers)
			{
				handler->OnClose(sender);
			}
		}

		void  EventHandler::OnConnect(std::shared_ptr<Session> sender)
		{
			for (auto& handler : m_connectEventHandlers)
			{
				handler->OnConnect(sender);
			}
		}

		void EventHandler::OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg)
		{
			for (auto& handler : m_dataEventHandlers)
			{
				handler->OnData(sender, msg);
			}
		}
	}
}
