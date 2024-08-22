#include "RPCServer.h"

namespace DSFramework {
	namespace DSRPC {
		RPCServer::RPCServer(ResponseDispatcher& responseDispatcher) : m_responseDispatcher(responseDispatcher) 
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

		void RPCServer::Execute(std::string serviceName, std::shared_ptr<RPCPacket> packet, RPCEventHandler& eventHandler, std::shared_ptr<Session> session)
		{
			if (m_serviceProcedures.find(serviceName) != m_serviceProcedures.end())
			{
				return;
			}
			ThreadPool::GetInstance()->Commit([this, serviceName, packet, &eventHandler, session]() {
				try {
					m_serviceProcedures[serviceName].second(packet, eventHandler, session);
					HandleCompleted(session, packet, eventHandler);
				}
				catch (std::exception& ex) {
					LOG_ERROR_CONSOLE(ex.what());
					HandleFailed(session, packet, eventHandler);
				}
				catch (...) {
					LOG_ERROR_CONSOLE("Execute failed: Unknown exception");
					HandleFailed(session, packet, eventHandler);
				}
			});
		}

		void RPCServer::HandleCompleted(std::shared_ptr<Session> sender, std::shared_ptr<RPCPacket> packet, RPCEventHandler& eventHandler)
		{
			LOG_DEBUG_CONSOLE("Execute completed");
			eventHandler.OnCompleted(packet->request_id());
			RPCPacketFactory::ChangeTypeToResponse(packet);
			RPCPacketFactory::SetErrorCode(packet, Packet::RPCPacketError::PKT_NO_ERROR);
			RPCPacketFactory::ChangeRecvSend(packet);
			m_responseDispatcher.PostRequestToQueue(sender, packet);
		}

		void RPCServer::HandleFailed(std::shared_ptr<Session> sender, std::shared_ptr<RPCPacket> packet, RPCEventHandler& eventHandler)
		{
			LOG_DEBUG_CONSOLE("Execute failed");
			eventHandler.OnFailed(packet->request_id());
			RPCPacketFactory::ChangeTypeToResponse(packet);
			RPCPacketFactory::SetErrorCode(packet, Packet::RPCPacketError::SERVICE_ERROR);
			RPCPacketFactory::ChangeRecvSend(packet);
			m_responseDispatcher.PostRequestToQueue(sender, packet);
		}
	}
}