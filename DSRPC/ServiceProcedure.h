#pragma once
#ifdef __GNUG__
#include <cxxabi.h>
#endif

#include <string>
#include <memory>
#include <atomic>
#include <vector>

#include "../DSCommunication/ThreadPool.h"

#include "RPCPacket.pb.h"
#include "RPCEventHandler.h"


using DSFramework::DSRPC::Packet::RPCPacket;
using DSFramework::DSRPC::RPCEventHandler;
namespace DSFramework {
	namespace DSRPC {
		class ServiceProcedure : public std::enable_shared_from_this<ServiceProcedure>
		{
		public:
			template<typename T>
			struct TypeWrapper {};
		protected:
			const std::string m_serviceName;
			const std::vector<std::string> m_parameterTypeList;
			const std::string m_resultType;
		private:
			template<typename T>
			std::string getTypeName() const {
				#ifdef __GNUG__
				// ��� GCC �� Clang
				int status;
				char* realname = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
				std::string name(realname);
				// �ͷŷ�����ڴ�
				free(realname);
				return name;
				// ��� MSVC
				#elif defined(_MSC_VER)
				return typeid(T).name();
				#endif
			}
		public:
			std::string GetServiceName() const { return m_serviceName; }
			const std::vector<std::string>& GetParameterTypeList() const { return m_parameterTypeList; }
		public:
			template<typename ReturnType = void, typename... Types>
			ServiceProcedure(std::string serviceName, TypeWrapper<ReturnType> result, TypeWrapper<Types>... paramstype) :
				m_serviceName(serviceName),
				m_resultType(getTypeName<ReturnType>()),
				m_parameterTypeList{ getTypeName<Types>()... } {}
			virtual ~ServiceProcedure() = default;
		public:
			virtual bool Invoke(std::shared_ptr<RPCPacket> packet) = 0;
			virtual bool CheckParameters(std::shared_ptr<RPCPacket> packet) = 0;
		};
	}
}