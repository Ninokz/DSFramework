#pragma once
#ifdef _MSC_VER
#define _WIN32_WINNT 0x0601
#endif

#include <mutex>
#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>

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

		class IEventHandler
		{
		public:
			IEventHandler() = default;
			virtual ~IEventHandler() = default;

			virtual void OnClose(std::shared_ptr<Session> sender) = 0;
			virtual void OnConnect(std::shared_ptr<Session> sender) = 0;
			virtual void OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg) = 0;
		};

		class EventHandler : public IEventHandler
		{
		protected:
			std::vector<std::shared_ptr<ICloseEventHandler>> m_closeEventHandlers;
			std::vector<std::shared_ptr<IConnectEventHandler>> m_connectEventHandlers;
			std::vector<std::shared_ptr<IDataEventHandler>> m_dataEventHandlers;
		public:
			EventHandler() = default;
			virtual ~EventHandler();

			void AddCloseEventHandler(std::shared_ptr<ICloseEventHandler> handler);

			void AddConnectEventHandler(std::shared_ptr<IConnectEventHandler> handler);

			void AddDataEventHandler(std::shared_ptr<IDataEventHandler> handler);

			virtual void OnClose(std::shared_ptr<Session> sender) override;

			virtual void OnConnect(std::shared_ptr<Session> sender) override;

			virtual void OnData(std::shared_ptr<Session> sender, std::shared_ptr<DSCRecvPacket> msg) override;
		};
	}
}
