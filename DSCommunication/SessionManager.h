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
#include "IOServicePool.h"
#include "DSCPacket.h"
#include "EventHandler.h"
#include "Logger.h"

using DSFramework::DSComponent::Logger;
using DSFramework::DSComponent::Log;

using DSFramework::DSComponent::IOServicePool;
namespace DSFramework {
	namespace DSCommunication {

		class SessionManager : public ICloseEventHandler, public IConnectEventHandler
		{
		protected:
			size_t m_maxSessionCount;
			std::unordered_map<std::string, std::shared_ptr<Session>> m_sessions;
		public:
			SessionManager(size_t maxSize);
			virtual ~SessionManager();

			virtual void OnClose(std::shared_ptr<Session> sender) override;
			virtual void OnConnect(std::shared_ptr<Session> sender) override;
		private:
			bool AddSession(std::shared_ptr<Session> session);
			void RemoveSession(std::string& uuid);
		};
	}
}