#include "RPCServer.h"

namespace DSFramework {
	namespace DSRPC {
		RPCServer::RPCServer()
		{
		
		}

		void RPCServer::RegisterService(std::string serviceName, CheckFunction checkFunction, ExecuteFunction executeFunction)
		{
			if (m_serviceProcedures.find(serviceName) == m_serviceProcedures.end())
			{
				m_serviceProcedures[serviceName] = std::make_pair(checkFunction, executeFunction);
			}
		}

		bool RPCServer::SearchService(std::string serviceName)
		{
			return m_serviceProcedures.find(serviceName) != m_serviceProcedures.end();
		}

		bool RPCServer::CheckServiceParameter(std::string serviceName, std::shared_ptr<RPCPacket> packet)
		{
			if (m_serviceProcedures.find(serviceName) != m_serviceProcedures.end())
			{
				return m_serviceProcedures[serviceName].first(packet);
			}
			return false;
		}

		void RPCServer::Execute(std::string serviceName, std::shared_ptr<RPCPacket> packet, std::shared_ptr<Session> session)
		{
			if (m_serviceProcedures.find(serviceName) != m_serviceProcedures.end())
			{
				return;
			}
			ThreadPool::GetInstance()->Commit([this, serviceName, packet, session]() {
				try {
					m_serviceProcedures[serviceName].second(packet, session);
					HandleCompleted(session, packet);
				}
				catch (std::exception& ex) {
					LOG_ERROR_CONSOLE(ex.what());
					HandleFailed(session, packet);
				}
				catch (...) {
					LOG_ERROR_CONSOLE("Execute failed: Unknown exception");
					HandleFailed(session, packet);
				}
			});
		}

		void RPCServer::HandleCompleted(std::shared_ptr<Session> sender, std::shared_ptr<RPCPacket> packet)
		{
			LOG_DEBUG_CONSOLE("Execute completed");
		}

		void RPCServer::HandleFailed(std::shared_ptr<Session> sender, std::shared_ptr<RPCPacket> packet)
		{
			LOG_DEBUG_CONSOLE("Execute failed");
		}
	}
}