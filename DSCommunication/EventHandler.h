#pragma once
#include <memory>

#include "Session.h"

namespace DSFramework {
	namespace DSCommunication {
		
		class ICloseEventHandler
		{
		public:
			virtual void OnClose(std::shared_ptr<Session> sender) = 0;
		};

		class IConnectEventHandler
		{
		public:
			virtual void OnConnect(std::shared_ptr<Session> sender) = 0;
		};

		class IDataEventHandler
		{
		public:
			virtual void OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg) = 0;
		};

		class EventHandler : public std::enable_shared_from_this<EventHandler>
		{
		protected:
			std::vector<std::shared_ptr<ICloseEventHandler>> m_closeEventHandlers;
			std::vector<std::shared_ptr<IConnectEventHandler>> m_connectEventHandlers;
			std::vector<std::shared_ptr<IDataEventHandler>> m_dataEventHandlers;
		public:
			EventHandler();
			virtual ~EventHandler();

			void AddCloseEventHandler(std::shared_ptr<ICloseEventHandler> handler);

			void AddConnectEventHandler(std::shared_ptr<IConnectEventHandler> handler);

			void AddDataEventHandler(std::shared_ptr<IDataEventHandler> handler);

			void OnClose(std::shared_ptr<Session> sender);

			void OnConnect(std::shared_ptr<Session> sender);

			void OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg);
		};
	}
}
