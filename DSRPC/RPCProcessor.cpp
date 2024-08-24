#include "RPCProcessor.h"

namespace DSFramework {
	namespace DSRPC {
		RPCProcessor::RPCProcessor(RPCEventHandler& m_rpcEventHandler) : m_rpcEventHandler(m_rpcEventHandler)
		{
		}

		void RPCProcessor::RegisterService(std::string serviceName, CheckFunction checkFunction, ExecuteFunction executeFunction)
		{
			if (m_serviceProcedures.find(serviceName) == m_serviceProcedures.end())
			{
				m_serviceProcedures[serviceName] = std::make_pair(checkFunction, executeFunction);
			}
		}

		void RPCProcessor::OnCommited(const std::shared_ptr<Session> sender, std::shared_ptr<RPCPacket> request)
		{
			const std::string& service = request->service();
			if (!CheckRequestService(request))
			{
				LOG_DEBUG_CONSOLE("Request Empty");
				m_rpcEventHandler.OnServiceEmptyRequest(sender, request);
				return;
			}
			if (!SearchService(service))
			{
				LOG_DEBUG_CONSOLE("Request Service Not found");
				m_rpcEventHandler.OnServiceNotFound(sender, request);
				return;
			}
			if (!CheckServiceParameter(service, request))
			{
				LOG_DEBUG_CONSOLE("Request Service Parameter invalid");
				m_rpcEventHandler.OnServiceParameterInvalid(sender, request);
				return;
			}
			Execute(service, request, sender);
		}

		bool RPCProcessor::CheckRequestService(std::shared_ptr<RPCPacket> packet)
		{
			if (packet->service().empty()) {
				return false;
			}
			return true;
		}

		bool RPCProcessor::SearchService(std::string serviceName)
		{
			return m_serviceProcedures.find(serviceName) != m_serviceProcedures.end();
		}

		bool RPCProcessor::CheckServiceParameter(std::string serviceName, std::shared_ptr<RPCPacket> packet)
		{
			if (m_serviceProcedures.find(serviceName) != m_serviceProcedures.end())
			{
				return m_serviceProcedures[serviceName].first(packet);
			}
			return false;
		}

		void RPCProcessor::Execute(std::string serviceName, std::shared_ptr<RPCPacket> packet, std::shared_ptr<Session> session)
		{
			ThreadPool::GetInstance()->Commit([this, serviceName, packet, session]() {
				try {
					m_serviceProcedures[serviceName].second(packet);
					m_rpcEventHandler.OnCompleted(session, packet);
				}
				catch (std::exception& ex) {
					LOG_ERROR_CONSOLE(ex.what());
					m_rpcEventHandler.OnServiceError(session, packet);
				}
				});
		}
	}
}